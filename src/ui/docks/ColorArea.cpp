/*******************************************************************************
** Copyright (C) 2024  CapyArt Studio                                         **
**                                                                            **
** This program is free software: you can redistribute it and/or modify       **
** it under the terms of the GNU General Public License as published by       **
** the Free Software Foundation, either version 3 of the License, or          **
** (at your option) any later version.                                        **
**                                                                            **
** This program is distributed in the hope that it will be useful,            **
** but WITHOUT ANY WARRANTY; without even the implied warranty of             **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              **
** GNU General Public License for more details.                               **
**                                                                            **
** You should have received a copy of the GNU General Public License          **
** along with this program.  If not, see <https://www.gnu.org/licenses/>.     **
*******************************************************************************/

#include "ColorArea.hpp"
#include "ui_ColorArea.h"
#include "ui/widgets/utils/MessageBoxUtils.hpp"
#include <fmt/format.h>
#include "io/ConsoleLogger.hpp"
#include "ui/widgets/delegates/ColorRectangleDelegate.hpp"
#include <QMessageBox>
#include <QInputDialog>
#include <filesystem>
#include "io/ApplicationFilesystem.hpp"

namespace capy::ui {
ColorArea::ColorArea(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ColorArea),
  _paletteModel(this),
  _colorTableModel(this) {
  ui->setupUi(this);

  _colorPicker = ui->colorPickerWidget;

  _colorPicker->setColor(QColor(0, 0, 0, 255)); // TODO: prob delete this and sync on startup
  connect(_colorPicker, &DefaultColorPicker::colorChanged, this,
    // TODO: Maybe can connect straight to signal slot?
          [&](const QColor& color) {
            emit colorPickerColorChanged(color);
          });

  connect(ui->addColorToPalette, &QPushButton::clicked, this,
    &ColorArea::addColorToPaletteClicked);

  ui->paletteComboBox->setModel(&_paletteModel);
  ui->colorTableView->setModel(&_colorTableModel);
  ui->colorTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

  auto* colorTableDelegate = new ColorRectangleDelegate(ui->colorTableView);
  ui->colorTableView->setItemDelegateForColumn(
    static_cast<int>(models::PaletteColorTableModel::ColumnName::Color),
    colorTableDelegate);

  connect(ui->paletteComboBox, &QComboBox::currentIndexChanged, this,
    &ColorArea::currentColorPaletteChanged);
  connect(ui->paletteComboBox, &QComboBox::activated, this,
    &ColorArea::userCurrentColorPaletteChanged);
  connect(ui->createPaletteButton, &QPushButton::clicked, this,
    &ColorArea::createPaletteClicked);
  connect(ui->removePaletteButton, &QPushButton::clicked, this,
    &ColorArea::removePaletteClicked);
  connect(ui->colorTableView, &QTableView::doubleClicked, this,
    &ColorArea::colorClicked);
  connect(ui->createColorButton, &QPushButton::clicked, this,
    &ColorArea::createColorClicked);
  connect(ui->removeColorButton, &QPushButton::clicked, this,
    &ColorArea::removeColorClicked);

  loadPalettesFromFilesystem();
}

ColorArea::~ColorArea() {
  delete ui;
}

void ColorArea::addColorToPaletteClicked() {
  const auto currentPaltteIndex = ui->paletteComboBox->currentIndex();
  if (currentPaltteIndex == -1) {
    return execMessageBox(this, QMessageBox::Warning, "Please create a palette first before adding colors");
  }

  const QColor colorToAdd = _colorPicker->getColor();
  const QString hint = QInputDialog::getText(this, "Add new Color to palette", "Color hint");
  const auto colorAddingError = _paletteModel.addColorToPalette(currentPaltteIndex, colorToAdd, hint.isEmpty() ? std::nullopt : std::optional(hint.toStdString()));
  if (colorAddingError.has_value()) {
    return execMessageBox(this, QMessageBox::Critical,
      "Error when adding color to palette: " + QString::fromStdString(colorAddingError.value()));
  }

  loadPalettesFromFilesystem();
}

void ColorArea::loadPalettesFromFilesystem() {
  // TODO: only load json files and validate them
  // TODO: load and _paleteModel.setPalettes() and if error then show error box
  const auto paletteFileList = listFilesInPath(FilesystemPath::Palettes);

  std::vector<Palette> palettes;
  for (const auto& paletteFilePath : paletteFileList) {
    const auto loadedPaletteOpt = Palette::createFromJson(paletteFilePath);
    if (!loadedPaletteOpt.has_value()) {
      execMessageBox(this, QMessageBox::Critical,
        "Error when loading palette: " + QString::fromStdString(loadedPaletteOpt.error()));
      continue;
    }

    const auto& loadedPalette = loadedPaletteOpt.value();
    logger::info(fmt::format("Loaded new palette: {}", loadedPalette.getName()));
    palettes.push_back(loadedPalette);
  }

  _paletteModel.setPalettes(std::move(palettes));

  if (isPaletteComboBoxIndexRestorable()) {
    ui->paletteComboBox->setCurrentIndex(_savedPaletteComboBoxIndex);
  }
}

void ColorArea::createPaletteClicked() {
  QString paletteName = QInputDialog::getText(this, "New Palette", "Palette name");
  if (paletteName.isEmpty()) {
    return;
  }

  if (_paletteModel.doesPaletteExist(paletteName.toStdString())) {
    return execMessageBox(this, QMessageBox::Warning, "Palette with this name already exists");
  }

  auto newPalette = Palette(paletteName.toStdString());

  const auto palettesPath = std::filesystem::path(getFilesystemPath(FilesystemPath::Palettes));
  const auto newPaletteFilePath = palettesPath / (newPalette.getName() + ".json");

  const auto saveError = newPalette.saveToJson(newPaletteFilePath.string());
  if (saveError.has_value()) {
    QMessageBox messageBox;
    return execMessageBox(this, QMessageBox::Critical,
      "Unable to save new palette, check filesystem permissions");
  } else {
    logger::info(fmt::format("Created new palette: {}", newPalette.getName()));
    loadPalettesFromFilesystem();
  }

  // TODO: Change index to newly created palette
}

void ColorArea::removePaletteClicked() {
  const auto paletteIndex = ui->paletteComboBox->currentIndex();
  if (paletteIndex == -1) {
    return;
  }

  if (!showConfirmationDialog(this, "Are you sure you want to delete this palette?")) {
    return;
  }

  logger::info(fmt::format("Attempting to remove palette at index: {}", paletteIndex));

  const auto& paletteToDelete = _paletteModel.getPalette(paletteIndex);
  std::optional<std::string> pathToDeleteOpt = paletteToDelete.getPath();
  if (!pathToDeleteOpt.has_value()) {
    logger::error(fmt::format("Unable to remove palette: {} due to missing path", paletteToDelete.getName()), logger::Severity::Severe);
    return;
  }

  const std::string pathToDelete = pathToDeleteOpt.value();
  if (!std::filesystem::remove(pathToDelete)) {
    logger::error(fmt::format("Unable to remove palette file at path: {}", pathToDelete), logger::Severity::Severe);
    return;
  }

  logger::info(fmt::format("Deleted palette: {}", paletteToDelete.getName()));
  loadPalettesFromFilesystem();
}

void ColorArea::createColorClicked() const {
  // TODO
}

void ColorArea::removeColorClicked() {
  const auto paletteIndex = ui->paletteComboBox->currentIndex();
  if (paletteIndex == -1) {
    return;
  }

  const QItemSelectionModel* selectionModel = ui->colorTableView->selectionModel();
  if (selectionModel->hasSelection()) {
    if (!showConfirmationDialog(this, "Are you sure you want to delete selected colors?")) {
      return;
    }

    const auto selectedRows = selectionModel->selectedRows();
    for (const auto selectedRow : selectedRows) {
      const auto colorIndex = selectedRow.row();
      const auto removeError = _paletteModel.removeColorFromPalette(paletteIndex, colorIndex);
      // TODO: Below -> maybe add a signal from colorTableModel to paletteModel or other way around to signal this without calling this method
      _colorTableModel.notifyThatColorWasRemovedFromThePalette(colorIndex);
      if (removeError.has_value()) {
        return execMessageBox(this, QMessageBox::Critical,
          "Error when updating palette after removing color: " + QString::fromStdString(removeError.value()));
        // TODO: Readd colors? So there is no discrepancy between ui and file
      }
    }
  }
}

void ColorArea::userCurrentColorPaletteChanged(const int newPaletteIndex){
  _savedPaletteComboBoxIndex = newPaletteIndex;
}

void ColorArea::colorClicked(const QModelIndex& index) const {
  const auto color = _paletteModel.getColor(ui->paletteComboBox->currentIndex(), index.row());
  _colorPicker->setColor(color.color);
}

void ColorArea::currentColorPaletteChanged(const int newPaletteIndex){
  const auto colorsOfCurrentPalette = _paletteModel.getColors(newPaletteIndex);
  _colorTableModel.setColors(colorsOfCurrentPalette);
}

bool ColorArea::isPaletteComboBoxIndexRestorable() const {
  return _savedPaletteComboBoxIndex != -1 && _savedPaletteComboBoxIndex < ui->paletteComboBox->count();
}
}

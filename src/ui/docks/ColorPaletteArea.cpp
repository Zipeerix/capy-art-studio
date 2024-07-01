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

#include <QMessageBox>
#include <QInputDialog>
#include "ColorPaletteArea.hpp"

#include <fmt/format.h>
#include <filesystem>
#include "io/ConsoleLogger.hpp"

#include "io/ApplicationFilesystem.hpp"
#include "ui_ColorPaletteArea.h"
#include "ui/widgets/delegates//ColorRectangleDelegate.hpp"

namespace capy::ui {
ColorPaletteArea::ColorPaletteArea(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ColorPaletteArea),
  _paletteModel(this),
  _colorTableModel(this) {
  ui->setupUi(this);

  ui->paletteComboBox->setModel(&_paletteModel);

  ui->colorTableView->setModel(&_colorTableModel);

  auto* colorTableDelegate = new ColorRectangleDelegate(ui->colorTableView);
  ui->colorTableView->setItemDelegateForColumn(
    static_cast<int>(models::PaletteColorTableModel::ColumnName::Color),
    colorTableDelegate);

  connect(ui->paletteComboBox, &QComboBox::currentIndexChanged, this,
    &ColorPaletteArea::currentColorPaletteChanged);
  connect(ui->createPaletteButton, &QPushButton::clicked, this,
    &ColorPaletteArea::createPaletteClicked);
  connect(ui->removePaletteButton, &QPushButton::clicked, this,
    &ColorPaletteArea::removePaletteClicked);

  loadPalettesFromFilesystem();
}

ColorPaletteArea::~ColorPaletteArea() {
    delete ui;
}

void ColorPaletteArea::loadPalettesFromFilesystem() {
  // TODO: load and _paleteModel.setPalettes() and if error then show error box
  const auto paletteFileList = listFilesInPath(FilesystemPath::Palettes);

  std::vector<Palette> palettes;
  for (const auto& paletteFilePath : paletteFileList) {
    const auto loadedPaletteOpt = Palette::fromJson(paletteFilePath);
    if (!loadedPaletteOpt.has_value()) {
      QMessageBox messageBox;
      messageBox.warning(this,"Error",
        "Error when loading palette: {}" + QString::fromStdString(loadedPaletteOpt.error()));
      messageBox.setFixedSize(500,200);
      messageBox.exec();
      continue;
    }

    const auto loadedPalette = loadedPaletteOpt.value();
    logger::info(fmt::format("Loaded new palette: {}", loadedPalette.getName()));
    palettes.push_back(loadedPalette);
  }

  _paletteModel.setPalettes(std::move(palettes));
}

void ColorPaletteArea::createPaletteClicked() {
  QString paletteName = QInputDialog::getText(this, "New Palette", "Palette name");
  const auto newPalette = Palette(paletteName.toStdString());

  const auto palettesPath = std::filesystem::path(getFilesystemPath(FilesystemPath::Palettes));
  const auto newPaletteFilePath = palettesPath / (newPalette.getName() + ".json");

  const auto saveResult = newPalette.saveToJson(newPaletteFilePath.string());
  if (!saveResult.has_value()) {
    QMessageBox messageBox;
    messageBox.warning(this,"Error",
      "Unable to save new palette, check filesystem permissions");
    messageBox.setFixedSize(500,200);
    messageBox.exec();
  } else {
    logger::info(fmt::format("Created new palette: {}", newPalette.getName()));
    loadPalettesFromFilesystem();
  }
}

void ColorPaletteArea::removePaletteClicked() {
  const auto paletteIndex = ui->paletteComboBox->currentIndex();
  if (paletteIndex == -1) {
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

void ColorPaletteArea::currentColorPaletteChanged(int newPaletteIndex){
  const auto colorsOfCurrentPalette = _paletteModel.getColors(newPaletteIndex);
  _colorTableModel.setColors(colorsOfCurrentPalette);
}
} // namespace capy::ui

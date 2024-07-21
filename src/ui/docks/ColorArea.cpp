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

#include <fmt/format.h>

#include <filesystem>
#include <QInputDialog>
#include <QMessageBox>

#include "io/ApplicationFilesystem.hpp"
#include "io/ConsoleLogger.hpp"
#include "ui/utils/MessageBoxUtils.hpp"
#include "ui/widgets/delegates/ColorRectangleDelegate.hpp"
#include "ui_ColorArea.h"

namespace capy::ui
{
ColorArea::ColorArea(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ColorArea),
    _palettesManager(this)
{
  ui->setupUi(this);

  _colorPicker = ui->colorPickerWidget;

  _colorPicker->setColor(QColor(0, 0, 0, 255)); // TODO: prob delete this and sync on startup
  connect(_colorPicker, &DefaultColorPicker::colorChanged, this,
          // TODO: Maybe can connect straight to signal slot?
          [&](const QColor& color) {
            emit colorPickerColorChanged(color);
          });

  connect(ui->addColorToPalette, &QPushButton::clicked, this, &ColorArea::addColorToPaletteClicked);

  ui->paletteComboBox->setModel(_palettesManager.getPaletteModel());
  ui->colorTableView->setModel(_palettesManager.getPaletteColorTableModel());
  ui->colorTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

  auto* colorTableDelegate = new ColorRectangleDelegate(ui->colorTableView);
  ui->colorTableView->setItemDelegateForColumn(
          static_cast<int>(models::PaletteColorTableModel::ColumnName::Color), colorTableDelegate);

  connect(ui->paletteComboBox, &QComboBox::currentIndexChanged, this,
          &ColorArea::currentColorPaletteChanged);
  connect(ui->paletteComboBox, &QComboBox::activated, this,
          &ColorArea::userCurrentColorPaletteChanged);
  connect(ui->createPaletteButton, &QPushButton::clicked, this, &ColorArea::createPaletteClicked);
  connect(ui->removePaletteButton, &QPushButton::clicked, this, &ColorArea::removePaletteClicked);
  connect(ui->colorTableView, &QTableView::doubleClicked, this, &ColorArea::colorClicked);
  connect(ui->createColorButton, &QPushButton::clicked, this, &ColorArea::createColorClicked);
  connect(ui->removeColorButton, &QPushButton::clicked, this, &ColorArea::removeColorClicked);

  connect(&_palettesManager, &PalettesManager::palettesUpdated, this, &ColorArea::updatePalettesUi);

  _palettesManager.loadPalettesFromFilesystem();
}

ColorArea::~ColorArea()
{
  delete ui;
}

void ColorArea::addColorToPaletteClicked()
{
  const auto currentPaltteIndex = ui->paletteComboBox->currentIndex();
  if (currentPaltteIndex == -1)
  {
    return execMessageBox(this, QMessageBox::Warning,
                          "Please create a palette first before adding colors");
  }

  const QColor colorToAdd = _colorPicker->getColor();
  const QString hint = QInputDialog::getText(this, "Add new Color to palette", "Color hint");
  const auto hintAsOptional = hint.isEmpty() ? std::nullopt : std::optional(hint.toStdString());

  _palettesManager.addColorToPalette(
          currentPaltteIndex, colorToAdd, hintAsOptional, [&](const std::string& error) {
            return execMessageBox(this, QMessageBox::Critical, QString::fromStdString(error));
          });
}

void ColorArea::updatePalettesUi() const
{
  if (isPaletteComboBoxIndexRestorable())
  {
    ui->paletteComboBox->setCurrentIndex(_savedPaletteComboBoxIndex);
  }
}

void ColorArea::createPaletteClicked()
{
  const QString paletteName = QInputDialog::getText(this, "New Palette", "Palette name");
  if (paletteName.isEmpty())
  {
    return;
  }

  _palettesManager.createPalette(paletteName.toStdString(), [&](const std::string& error) {
    return execMessageBox(this, QMessageBox::Warning, QString::fromStdString(error));
  });

  // TODO: Change index to newly created palette
}

void ColorArea::removePaletteClicked()
{
  const auto paletteIndex = ui->paletteComboBox->currentIndex();
  if (paletteIndex == -1)
  {
    return;
  }

  if (!showConfirmationDialog(this, "Are you sure you want to delete this palette?"))
  {
    return;
  }

  _palettesManager.removePalette(paletteIndex, [&](const std::string& error) {
    return execMessageBox(this, QMessageBox::Warning, QString::fromStdString(error));
  });
}

void ColorArea::createColorClicked() const
{
  // TODO
}

void ColorArea::removeColorClicked()
{
  const auto paletteIndex = ui->paletteComboBox->currentIndex();
  if (paletteIndex == -1)
  {
    return;
  }

  const QItemSelectionModel* selectionModel = ui->colorTableView->selectionModel();
  if (selectionModel->hasSelection())
  {
    if (!showConfirmationDialog(this, "Are you sure you want to delete selected colors?"))
    {
      return;
    }

    const auto selectedRows = selectionModel->selectedRows();
    for (const auto selectedRow: selectedRows)
    {
      const auto colorIndex = selectedRow.row();
      _palettesManager.removeColorFromPalette(
              paletteIndex, colorIndex, [&](const std::string& error) {
                return execMessageBox(this, QMessageBox::Critical, QString::fromStdString(error));
              });
    }
  }
}

void ColorArea::userCurrentColorPaletteChanged(const int newPaletteIndex)
{
  _savedPaletteComboBoxIndex = newPaletteIndex;
}

void ColorArea::colorClicked(const QModelIndex& index) const
{
  const auto paletteColor =
          _palettesManager.getColorFromPalette(ui->paletteComboBox->currentIndex(), index.row());
  _colorPicker->setColor(paletteColor.color);
}

void ColorArea::currentColorPaletteChanged(const int newPaletteIndex)
{
  _palettesManager.setTableColorsFromPalette(newPaletteIndex);
}

bool ColorArea::isPaletteComboBoxIndexRestorable() const
{
  return _savedPaletteComboBoxIndex != -1 &&
         _savedPaletteComboBoxIndex < ui->paletteComboBox->count();
}
} // namespace capy::ui

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

#include "ColorPaletteArea.hpp"
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
}

ColorPaletteArea::~ColorPaletteArea() {
    delete ui;
}

void ColorPaletteArea::currentColorPaletteChanged(int newPaletteIndex){
  // TODO get data from model and update the widget in ui
  const auto colorsOfCurrentPalette = _paletteModel.getColors(newPaletteIndex);
  _colorTableModel.setColors(colorsOfCurrentPalette);
  // update model? or is it updated
}
} // namespace capy::ui

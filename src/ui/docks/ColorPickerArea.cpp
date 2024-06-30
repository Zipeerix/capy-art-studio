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

#include "ColorPickerArea.hpp"
#include "ui_ColorPickerArea.h"

namespace capy::ui {
ColorPickerArea::ColorPickerArea(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ColorPickerArea) {
  ui->setupUi(this);
  _colorPicker = ui->colorPickerWidget;

  _colorPicker->setColor(QColor(0, 0, 0, 255)); // TODO: prob delete this and sync on startup
  connect(_colorPicker, &DefaultColorPicker::colorChanged, this,
    // TODO: Maybe can connect straight to signal slot?
          [&](const QColor& color) {
            emit colorPickerColorChanged(color);
          });
}

ColorPickerArea::~ColorPickerArea() {
    delete ui;
}
} // namespace capy::ui

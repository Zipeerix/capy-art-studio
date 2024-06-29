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

#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

#include <QDialog>
#include <QLabel>
#include "ColorPickerSlider.hpp"

namespace capy::ui {
class ColorPicker final : public QDialog {
  Q_OBJECT

 public:
  explicit ColorPicker(QWidget* parent = nullptr);
  ~ColorPicker() override;

  void setColor(QColor color);

  void updateShownColor();

 public slots:
  void addToColorPaletteClicked();

  signals:
    void colorChanged(QColor newColor);

 private:
  ColorPickerSlider* _hueSlider;
  ColorPickerSlider* _saturationSlider;
  ColorPickerSlider* _brightnessSlider;
  ColorPickerSlider* _alphaSlider;

  QLabel* _hexLabel;
  QLabel* _colorShowcase;

  QColor _selectedColor;
};
}  // namespace capy::ui

#endif  // COLORPICKER_HPP

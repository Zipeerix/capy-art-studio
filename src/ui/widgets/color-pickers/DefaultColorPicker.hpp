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

#ifndef DEFAULT_COLORPICKER_HPP
#define DEFAULT_COLORPICKER_HPP

#include <QLabel>

#include "DefaultColorPickerSlider.hpp"
#include "IBaseColorPicker.hpp"

namespace capy::ui
{
class DefaultColorPicker final : public QWidget
{
  Q_OBJECT
public:
  explicit DefaultColorPicker(QWidget* parent = nullptr);
  ~DefaultColorPicker() override = default;

  void setColor(QColor color);
  void setColor(int hue, int saturation, int brightness, int alpha);
  QColor getColor() const;

  void updateShownColor() const;

signals:
  void colorChanged(QColor newColor);

private:
  DefaultColorPickerSlider* _hueSlider;
  DefaultColorPickerSlider* _saturationSlider;
  DefaultColorPickerSlider* _brightnessSlider;
  DefaultColorPickerSlider* _alphaSlider;

  QLabel* _hexLabel;
  QLabel* _colorShowcase;

  QColor _selectedColor;
};
} // namespace capy::ui

#endif // DEFAULT_COLORPICKER_HPP

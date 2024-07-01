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

#ifndef COLORPICKERSLIDER_HPP
#define COLORPICKERSLIDER_HPP

#include <QGradientStops>
#include <QSlider>
#include <functional>

#include "ui/widgets/utils/CheckerboardPixmap.hpp"

namespace capy::ui {
class DefaultColorPickerSlider final : public QSlider {
  Q_OBJECT

 public:
  explicit DefaultColorPickerSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
  explicit DefaultColorPickerSlider(QWidget* parent = nullptr);

  ~DefaultColorPickerSlider() override;

  void setGradientStops(QGradientStops gradientStops);
  void setRenderCheckerboard(bool renderCheckerboard);

  void paintEvent(QPaintEvent* event) override;

 private:
  QGradientStops _gradientStops;
  bool _renderCheckerboard = false;
  CheckerboardPixmap _checkerboardPixmap;
};
}  // namespace capy::ui

#endif  // COLORPICKERSLIDER_HPP

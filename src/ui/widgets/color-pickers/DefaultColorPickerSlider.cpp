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

#include "DefaultColorPickerSlider.hpp"

#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>

namespace capy::ui
{
DefaultColorPickerSlider::DefaultColorPickerSlider(Qt::Orientation orientation, QWidget* parent) :
    QSlider(orientation, parent)
{
}

DefaultColorPickerSlider::DefaultColorPickerSlider(QWidget* parent) :
    DefaultColorPickerSlider(Qt::Vertical, parent)
{
}

void DefaultColorPickerSlider::setGradientStops(QGradientStops gradientStops)
{
  _gradientStops = std::move(gradientStops);
  update();
}

void DefaultColorPickerSlider::setRenderCheckerboard(const bool renderCheckerboard)
{
  _renderCheckerboard = renderCheckerboard;
  update();
}

void DefaultColorPickerSlider::paintEvent([[maybe_unused]] QPaintEvent* event)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QLinearGradient gradient;
  QRect drawingRect = rect();

  float roundRectRadius = 0;
  float handleRadius = 0;
  QPointF handlePos;

  switch (orientation())
  {
    case Qt::Horizontal: {
      drawingRect.adjust(height() / 2, 2, -height() / 2, -2);
      gradient = QLinearGradient(0, 0, drawingRect.width(), 0);
      roundRectRadius = drawingRect.height() / 2;
      handleRadius = height() / 2;
      handlePos = {static_cast<qreal>(sliderPosition()) * drawingRect.width() / maximum() +
                           height() / 2,
                   static_cast<qreal>(height()) / 2};
      break;
    }

    case Qt::Vertical: {
      drawingRect.adjust(2, width() / 2, -2, -width() / 2);
      gradient = QLinearGradient(0, 0, 0, drawingRect.height());
      roundRectRadius = drawingRect.width() / 2;
      handleRadius = width() / 2;
      handlePos = {static_cast<qreal>(width()) / 2,
                   static_cast<qreal>(sliderPosition()) * drawingRect.width() / maximum() +
                           width() / 2};
      break;
    }
  };

  gradient.setStops(_gradientStops);

  if (_renderCheckerboard)
  {
    painter.setBrush(QBrush(_checkerboardPixmap));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(drawingRect, roundRectRadius, roundRectRadius);
  }

  painter.setPen(Qt::NoPen);
  painter.setBrush(gradient);
  painter.drawRoundedRect(drawingRect, roundRectRadius, roundRectRadius);

  // find the 2 colors that are closest to the handle position
  QColor leftColor = _gradientStops.isEmpty() ? QColor() : _gradientStops.front().second;
  QColor rightColor = _gradientStops.isEmpty() ? QColor() : _gradientStops.back().second;
  float leftPos = 0;
  float rightPos = 1;
  float valueFloat =
          static_cast<float>(value() - minimum()) / static_cast<float>(maximum() - minimum());

  for (int i = 0; i < _gradientStops.size() - 1; ++i)
  {
    if (_gradientStops[i].first <= valueFloat && _gradientStops[i + 1].first >= valueFloat)
    {
      leftColor = _gradientStops[i].second;
      rightColor = _gradientStops[i + 1].second;
      leftPos = _gradientStops[i].first;
      rightPos = _gradientStops[i + 1].first;
      break;
    }
  }

  // interpolate between the 2 colors
  float handlePosInGradient = (valueFloat - leftPos) / (rightPos - leftPos);
  QColor fill = QColor::fromRgbF(
          leftColor.redF() + (rightColor.redF() - leftColor.redF()) * handlePosInGradient,
          leftColor.greenF() + (rightColor.greenF() - leftColor.greenF()) * handlePosInGradient,
          leftColor.blueF() + (rightColor.blueF() - leftColor.blueF()) * handlePosInGradient,
          leftColor.alphaF() + (rightColor.alphaF() - leftColor.alphaF()) * handlePosInGradient);

  painter.setPen(QPen(palette().color(QPalette::Text), 2));
  painter.setBrush(fill);
  painter.drawEllipse(handlePos, handleRadius - 1, handleRadius - 1);
}
} // namespace capy::ui
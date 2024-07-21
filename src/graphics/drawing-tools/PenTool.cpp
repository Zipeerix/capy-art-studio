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

#include "PenTool.hpp"
#include "ui/widgets/DrawingWidget.hpp"
#include "algorithms/Bresenham.hpp"

namespace capy {
PenTool::PenTool(ui::DrawingWidget* drawingWidget) :
  IDrawingTool(drawingWidget) {
}

void PenTool::setColor(const QColor newColor) {
  _color = newColor;
}

QColor PenTool::getColor() const {
  return _color;
}

void PenTool::onSwitchTo() {

}

void PenTool::onSwitchOutOf() {

}

bool PenTool::mousePressEvent([[maybe_unused]] QMouseEvent* event, const std::optional<QPoint>& clickedPixel) {
  if (clickedPixel.has_value()) {
    const auto clickedPixelX = clickedPixel->x();
    const auto clickedPixelY = clickedPixel->y();

    _drawingWidget->drawPixelOnBothRepresentations(clickedPixelX, clickedPixelY, _color);
    return true;
  }

  return false;
}

bool PenTool::mouseMoveEvent([[maybe_unused]] QMouseEvent* event, const std::optional<QPoint>& movingThroughPixel) {
  if (movingThroughPixel.has_value()) {
    const auto clickedPixelX = movingThroughPixel->x();
    const auto clickedPixelY = movingThroughPixel->y();

    if (_lastContinousDrawingPoint.has_value()) {
      // TODO: Move this to a method or cleanup/tool_to_class seperation
      static const auto pixelDrawingAction = [&](int x, int y) {
        _drawingWidget->drawPixelOnBothRepresentations(x, y, _color);
      };

      algorithms::applyBresenham(_lastContinousDrawingPoint->x(),
                                 _lastContinousDrawingPoint->y(),
                                 clickedPixelX,
                                 clickedPixelY,
                                 pixelDrawingAction);
    }

    _lastContinousDrawingPoint = movingThroughPixel.value();
    // pixelDrawingAction(clickedPixelX, clickedPixelY...)
  } else {
    _lastContinousDrawingPoint = std::nullopt;
  }

  return true;
}

bool PenTool::mouseReleaseEvent([[maybe_unused]] QMouseEvent* event, [[maybe_unused]] const std::optional<QPoint>& clickedPixel) {
  _lastContinousDrawingPoint = std::nullopt;
  return true;
}
} // capy
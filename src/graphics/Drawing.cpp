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

#include "Drawing.hpp"

namespace capy {
Drawing::Drawing(int width, int height) :
  _width(width), _height(height) {
  _layers.emplace_back(width, height);
  // TODO: Readd when testing multiple layers
  for (int i = 0; i < 1; i++) {
    _layers.emplace_back(width, height);
  }
}

int Drawing::getWidth() const {
  return _width;
}

int Drawing::getHeight() const {
  return _height;
}

const Layer& Drawing::getCurrentLayer() const {
  return _layers.at(_currentLayer);
}

void Drawing::setCurrentLayer(int newCurrentLayer) {
  _currentLayer = newCurrentLayer;
}

void Drawing::drawPixelOnCurrentLayer(int x, int y, const QColor& color) {
  auto& currentLayer = _layers.at(_currentLayer);
  currentLayer.drawPixel(x, y, color);
}

QColor Drawing::calculateCombinedPixelColor(int x, int y) const {
  //return QColor(128, 64, 128, 255);
  // TODO: Find last layer with non-zero opacity, later when opacitty is introduced it will be harder..
  // TODO: To improve performance look from the back and find FIRST
  auto currentColor = QColor(255, 255, 255, 255);
  for (const auto& layer : _layers) {
    auto pixel = layer.getPixel(x, y);
    if (pixel.isSolid()) {
      currentColor = pixel.convertToQColor();
    }
  }

  return currentColor;
}
} // capy
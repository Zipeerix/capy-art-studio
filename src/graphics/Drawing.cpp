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
Drawing::Drawing(int width, int height) : _width(width), _height(height) {
  _layers.emplace_back(width, height);
  // NOTE: For testing
  for (int i = 0; i < 3; i++) {
    _layers.emplace_back(width, height);
  }
}

int Drawing::getWidth() const { return _width; }

int Drawing::getHeight() const { return _height; }

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
  // TODO: idk how to move to class since the lambda fucking dies there
  algorithms::AlphaBlender alphaBlender([&](int x, int y, int layer) {
    return _layers.at(layer).getPixel(x, y);
  });  // tODO: if keeping it this way then pass layers size to constructor lol
  return alphaBlender.blend(x, y, _layers.size());
}
}  // namespace capy
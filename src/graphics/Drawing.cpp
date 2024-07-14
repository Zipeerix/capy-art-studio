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
Drawing::Drawing(const int width, const int height) : _width(width), _height(height) {
  _layers.emplace_back(width, height);
}

int Drawing::getWidth() const { return _width; }

int Drawing::getHeight() const { return _height; }

const Layer& Drawing::getCurrentLayer() const { return _layers.at(_currentLayer); }

void Drawing::setCurrentLayer(const int newCurrentLayer) { _currentLayer = newCurrentLayer; }

void Drawing::drawPixelOnCurrentLayerInternalRepresentationOnly(const int x, const int y, const QColor& color) {
  auto& currentLayer = _layers.at(_currentLayer);
  currentLayer.drawPixel(x, y, color);
}

QColor Drawing::calculateCombinedPixelColor(const int x, const int y) const {
  // TODO: Is it fine? Doesn't the lambda get fucked here?
  // TODO: if program crashes then its this retarded static here
  // TODO: change x to xArg and y to yArg and check in getPixel and test asap if layers work
  // transparently
  // TODO: Member?
  static algorithms::AlphaBlender alphaBlender(
      [&](const int xPixel, const int yPixel, const int layer) {
        return _layers.at(layer).getPixel(xPixel, yPixel);
      });
  return alphaBlender.blend(x, y, _layers.size());
}
}  // namespace capy
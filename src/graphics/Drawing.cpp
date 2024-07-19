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

#include <stdexcept>
#include "Drawing.hpp"

namespace capy {
Drawing::Drawing(const int width, const int height) : _width(width), _height(height) {
  _layers.emplace_back(width, height, "Base Layer");
}

int Drawing::getWidth() const { return _width; }

int Drawing::getHeight() const { return _height; }

int Drawing::getLayerCount() const { return _layers.size(); }

const Layer& Drawing::getCurrentLayer() const { return _layers.at(_currentLayer); }

const std::vector<Layer>& Drawing::getLayers() const { return _layers; }

void Drawing::setCurrentLayer(const int newCurrentLayer) { _currentLayer = newCurrentLayer; }

void Drawing::insertOrAssignLayerFromRawPixels(const int index, const std::string& name,
                                               std::vector<Pixel> pixels) {
  if (static_cast<std::size_t>(index) == _layers.size()) {
    _layers.emplace_back(_width, _height, name);
  } else if (!(static_cast<std::size_t>(index) < _layers.size())) {
    throw std::logic_error("Bad index provided, check logic");
  }

  _layers.at(index).setPixels(std::move(pixels));
}

void Drawing::drawPixelOnCurrentLayerInternalRepresentationOnly(const int x, const int y,
                                                                const QColor& color) {
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
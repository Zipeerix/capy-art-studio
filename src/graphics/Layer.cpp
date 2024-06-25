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

#include "Layer.hpp"

namespace capy {
Layer::Layer(int width, int height) :
  _width(width), _height(height) {
  _pixels.resize(width * height, Pixel{0});
}

bool Layer::isVisible() const {
  return _visible;
}

void Layer::show() {
  _visible = true;
}

void Layer::hide() {
  _visible = false;
}

void Layer::drawPixel(int x, int y, const QColor& color) {
  auto& targetPixel = getMutablePixel(x, y);
  targetPixel.updateFromQColor(color);
}

/* TODO Combine two functions below */
const Pixel& Layer::getPixel(int x, int y) const {
  const int singleDimensionalIndex = y * _width + x;
  return _pixels.at(singleDimensionalIndex);
}

Pixel& Layer::getMutablePixel(int x, int y) {
  const int singleDimensionalIndex = y * _width + x;
  return _pixels.at(singleDimensionalIndex);
}
} // capy
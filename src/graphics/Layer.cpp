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

#include "utils/General.hpp"

namespace capy {
Layer::Layer(const int width, const int height, std::string name)
    : _name(std::move(name)), _dimensions(width, height) {
  _pixels.resize(width * height, Pixel::white(constants::alpha::transparent));
}

bool Layer::isVisible() const { return _visible; }

void Layer::show() { _visible = true; }

void Layer::hide() { _visible = false; }

void Layer::setPixels(std::vector<Pixel> pixels) { _pixels = std::move(pixels); }

void Layer::setName(std::string name) { _name = std::move(name); }

std::string Layer::getName() const { return _name; }

utils::Dimensions Layer::getDimensions() const { return _dimensions; }

uint64_t Layer::calculateInMemorySize() const {
  return _pixels.size() * sizeof(Pixel) + sizeof(Layer);
}

void Layer::drawPixel(const int x, const int y, const QColor& color) {
  auto& targetPixel = getMutablePixel(x, y);
  targetPixel.updateFromQColor(color);
}

const Pixel& Layer::getPixel(const int x, const int y) const {
  return _pixels.at(utils::convert2DIndexto1DIndex(x, y, _dimensions.getWidth()));
}

const std::vector<Pixel>& Layer::getPixels() const { return _pixels; }

Pixel& Layer::getMutablePixel(const int x, const int y) {
  return _pixels.at(utils::convert2DIndexto1DIndex(x, y, _dimensions.getWidth()));
}
}  // namespace capy
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

#ifndef LAYER_HPP
#define LAYER_HPP

#include <QImage>

#include "Pixel.hpp"
#include "utils/Dimensions.hpp"

namespace capy
{
class Layer
{
public:
  Layer(int width, int height, std::string name);

  bool isVisible() const;
  void hide();
  void show();

  void setPixels(std::vector<Pixel> pixels);
  void setName(std::string name);
  std::string getName() const;

  utils::Dimensions getDimensions() const;

  const Pixel& getPixel(int x, int y) const;
  const std::vector<Pixel>& getPixels() const;
  void drawPixel(int x, int y, const QColor& color);

  uint64_t calculateInMemorySize() const;

private:
  std::string _name;
  utils::Dimensions _dimensions;
  std::vector<Pixel> _pixels;
  bool _visible = false;

  Pixel& getMutablePixel(int x, int y);
};
} // namespace capy

#endif // LAYER_HPP

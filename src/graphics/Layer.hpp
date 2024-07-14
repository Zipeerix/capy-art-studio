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

namespace capy {
class Layer {
 public:
  Layer(int width, int height, std::string name);

  bool isVisible() const;
  void hide();
  void show();

  void setPixels(std::vector<Pixel> pixels);
  void setName(std::string name);

  const Pixel& getPixel(int x, int y) const;
  void drawPixel(int x, int y, const QColor& color);

 private:
  std::string _name;
  std::vector<Pixel> _pixels;
  bool _visible = false;

  // TODO: remove when used
  [[maybe_unused]] int _width;
  [[maybe_unused]] int _height;

  Pixel& getMutablePixel(int x, int y);
};
}  // namespace capy

#endif  // LAYER_HPP

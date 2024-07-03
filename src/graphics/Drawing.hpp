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

#ifndef DRAWING_HPP
#define DRAWING_HPP

#include <vector>

#include "Layer.hpp"
#include "algorithms/AlphaBlending.hpp"

namespace capy {
class Drawing {
 public:
  Drawing(int width, int height);

  int getWidth() const;
  int getHeight() const;
  const Layer& getCurrentLayer() const;
  void setCurrentLayer(int newCurrentLayer);

  void drawPixelOnCurrentLayer(int x, int y, const QColor& color);
  QColor calculateCombinedPixelColor(int x, int y) const;

 private:
  std::vector<Layer> _layers;
  int _currentLayer = 0;
  int _width;
  int _height;
};
}  // namespace capy

#endif  // DRAWING_HPP

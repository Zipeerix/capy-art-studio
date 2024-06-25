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
}

int Drawing::getWidth() const {
  return _width;
}

int Drawing::getHeight() const {
  return _height;
}

QPixmap Drawing::convergeLayersIntoPixmap() const {
  QImage image(_width, _height, QImage::Format_RGBA8888);
  for (int x = 0; x < _width; x++) {
    for (int y = 0; y < _height; y++) {
      image.setPixelColor(QPoint(x, y), calculateConvergedPixelColor(x, y));
    }
  }

  return QPixmap::fromImage(image);
}

QColor Drawing::calculateConvergedPixelColor(int x, int y) const {
  return QColor(128, 64, 128, 255);
}
} // capy
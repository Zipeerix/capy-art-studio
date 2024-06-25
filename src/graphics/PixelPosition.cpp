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

#include "PixelPosition.hpp"
#include <QtCore/qmath.h>

namespace capy {
PixelPosition::PixelPosition(QPointF relativePosition, int pixelRatio) {
  _x = qFloor(relativePosition.x() / pixelRatio);
  _y = qFloor(relativePosition.y() / pixelRatio);
}

int PixelPosition::getX() const {
  return _x;
}

int PixelPosition::getY() const {
  return _y;
}

int PixelPosition::getSingleDimensionalIndex(int width) const {
  // TODO: Move below to help util function
  return _x * width + _y;
}

bool PixelPosition::isValid() const {
  return _x > 0 && _y > 0;
}
} // capy
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

#ifndef PIXELPOSITION_HPP
#define PIXELPOSITION_HPP

#include <QPoint>

namespace capy {
class PixelPosition {
public:
  explicit PixelPosition(QPointF point, int pixelRatio);

  [[nodiscard]] int getX() const;
  [[nodiscard]] int getY() const;
  [[nodiscard]] int getSingleDimensionalIndex(int width) const;

  [[nodiscard]] bool isValid() const;

private:
  int _x;
  int _y;
};
} // capy

#endif //PIXELPOSITION_HPP

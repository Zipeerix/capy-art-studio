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

#include "Bresenham.hpp"

#include <cmath>

namespace capy::algorithms {
static std::pair<int, int> calculateAbosluteDifference(int n0, int n1) {
  const int dn = n1 - n0;
  const int multiplicator = dn > 0 ? 1 : -1;
  return {multiplicator, std::abs(dn)};
}

void applyBresenham(int x0, int y0, int x1, int y1,
                    const CoordinateApplicationFunction& actionOnTarget) {
  const auto [sx, dx] = calculateAbosluteDifference(x0, x1);
  const auto [sy, dy] = calculateAbosluteDifference(y0, y1);
  int epsilon = 0;
  if (dx > dy) {
    for (auto x = x0, y = y0; sx < 0 ? x >= x1 : x <= x1; x += sx) {
      actionOnTarget(x, y);
      epsilon += dy;
      if (epsilon << 1 >= dx) {
        y += sy;
        epsilon -= dx;
      }
    }
  } else {
    for (auto x = x0, y = y0; sy < 0 ? y >= y1 : y <= y1; y += sy) {
      actionOnTarget(x, y);
      epsilon += dx;
      if (epsilon << 1 >= dy) {
        x += sx;
        epsilon -= dy;
      }
    }
  }
}

void applyBresenham(const QPoint& firstPoint, const QPoint& secondPoint,
                    const CoordinateApplicationFunction& actionOnTarget) {
  applyBresenham(firstPoint.x(), firstPoint.y(), secondPoint.x(),
                 secondPoint.y(), actionOnTarget);
}
}  // namespace capy::algorithms
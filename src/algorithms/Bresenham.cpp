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
void applyBresenham(int x0, int y0, int x1, int y1,
                    const CoordinateApplicationFunction& actionOnTarget) {
  /* TODO Refactor */
  /* TODO Read post maybe do original: https://www.middle-engine.com/blog/posts/2020/07/28/bresenhams-line-algorithm */
  /* TODO: I think orginal is better for 'exected' results for artist */
  const auto dx = x1 - x0;
  const auto dy = y1 - y0;

  const auto adx = (abs(dx) + 1) << 1;
  const auto ady = (abs(dy) + 1) << 1;

  const auto sx = dx > 0 ? 1 : -1;
  const auto sy = dy > 0 ? 1 : -1;

  if (adx > ady) {
    auto eps = (ady - adx) >> 1;
    for (auto x = x0, y = y0; sx < 0 ? x >= x1 : x <= x1; x += sx) {
      actionOnTarget(x, y);
      eps += ady;
      if (eps << 1 >= adx) {
        y += sy;
        eps -= adx;
      }
    }
  } else {
    auto eps = (adx - ady) >> 1;
    for (auto x = x0, y = y0; sy < 0 ? y >= y1 : y <= y1; y += sy) {
      actionOnTarget(x, y);
      eps += adx;
      if (eps << 1 >= ady) {
        x += sx;
        eps -= ady;
      }
    }
  }
}

void applyBresenham(const QPoint& firstPoint, const QPoint& secondPoint,
                    const CoordinateApplicationFunction& actionOnTarget) {
  applyBresenham(firstPoint.x(), firstPoint.y(), secondPoint.x(),
                 secondPoint.y(), actionOnTarget);
}
}
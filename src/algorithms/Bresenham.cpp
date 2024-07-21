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
#include <QPointF>
#include <utility>

namespace capy::algorithms
{
namespace
{
std::pair<int, int> calculateAbosluteDifference(const int firstPointX, const int secondPointX)
{
  const int diffN = secondPointX - firstPointX;
  const int multiplicator = diffN > 0 ? 1 : -1;
  return {multiplicator, std::abs(diffN)};
}
} // namespace

void applyBresenham(const int firstPointX, const int firstPointY, const int secondPointX,
                    const int secondPointY, const CoordinateApplicationFunction& actionOnTarget)
{
  const auto [sx, dx] = calculateAbosluteDifference(firstPointX, secondPointX);
  const auto [sy, dy] = calculateAbosluteDifference(firstPointY, secondPointY);
  int epsilon = 0;
  if (dx > dy)
  {
    for (auto x = firstPointX, y = firstPointY; sx < 0 ? x >= secondPointX : x <= secondPointX;
         x += sx)
    {
      actionOnTarget(x, y);
      epsilon += dy;
      if (epsilon << 1 >= dx)
      {
        y += sy;
        epsilon -= dx;
      }
    }
  }
  else
  {
    for (auto x = firstPointX, y = firstPointY; sy < 0 ? y >= secondPointY : y <= secondPointY;
         y += sy)
    {
      actionOnTarget(x, y);
      epsilon += dx;
      if (epsilon << 1 >= dy)
      {
        x += sx;
        epsilon -= dy;
      }
    }
  }
}

void applyBresenham(const QPoint& firstPoint, const QPoint& secondPoint,
                    const CoordinateApplicationFunction& actionOnTarget)
{
  applyBresenham(firstPoint.x(), firstPoint.y(), secondPoint.x(), secondPoint.y(), actionOnTarget);
}
} // namespace capy::algorithms

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

#ifndef BRESENHAM_HPP
#define BRESENHAM_HPP

#include <functional>
#include <QPoint>

namespace capy::algorithms
{
using CoordinateApplicationFunction = std::function<void(int x, int y)>;

void applyBresenham(const QPoint& firstPoint, const QPoint& secondPoint,
                    const CoordinateApplicationFunction& actionOnTarget);
void applyBresenham(int x0, int y0, int x1, int y1,
                    const CoordinateApplicationFunction& actionOnTarget);
} // namespace capy::algorithms

#endif // BRESENHAM_HPP

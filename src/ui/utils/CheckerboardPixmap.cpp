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

#include "CheckerboardPixmap.hpp"

#include <QPainter>

namespace capy::ui
{
CheckerboardPixmap::CheckerboardPixmap() :
    QPixmap(16, 16)
{
  fill(getCheckerboardFillColor());
  QPainter painter{this};

  painter.fillRect(0, 0, 8, 8, getCheckerboardRectangleColor());
  painter.fillRect(8, 8, 8, 8, getCheckerboardRectangleColor());
}

QColor CheckerboardPixmap::getCheckerboardFillColor()
{
  // TODO: in dark mode: Qt::black if not overriden in settings
  return Qt::white;
}

QColor CheckerboardPixmap::getCheckerboardRectangleColor()
{
  // TODO: in dark mode: Qt::darkGray if not overriden in settings
  return Qt::lightGray;
}
} // namespace capy::ui
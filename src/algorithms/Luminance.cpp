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

#include "Luminance.hpp"

namespace capy::algorithms {
constexpr double LUMINANCE_RED_CONSTANT = 0.299;
constexpr double LUMINANCE_GREEN_CONSTANT = 0.587;
constexpr double LUMINANCE_BLUE_CONSTANT = 0.114;

int calculateLuminance(const QColor& color) {
  return static_cast<int>(
         LUMINANCE_RED_CONSTANT * color.red() +
         LUMINANCE_GREEN_CONSTANT * color.green() +
         LUMINANCE_BLUE_CONSTANT * color.blue());
}

QColor blackOrWhiteBasedOnLuminance(const QColor& color){
  return calculateLuminance(color) > 128 ? Qt::black : Qt::white;
}

}
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

#ifndef GENERAL_HPP
#define GENERAL_HPP

#include <cstdint>

// TODO: Change this and 'Memory.hpp' namespace to capy::utils?
namespace capy::utils {
namespace constants {
constexpr char nullTerminator = 0;
}

uint64_t calculateInMemorySizeOfImage(int width, int height, int layers = 1);
int convert2DIndexto1DIndex(int x, int y, int width);
}  // namespace capy::utils

#endif  // GENERAL_HPP

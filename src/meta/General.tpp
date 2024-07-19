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

#ifndef GENERAL_TPP
#define GENERAL_TPP

#include <memory>
#include <stdexcept>

namespace capy {
uint64_t calculateInMemorySizeOfImage(int width, int height, int layers = 1);

template <typename T, typename U>
static void compileTimeTypeCheck() {
  // TODO: This probably doesn't work, check, maybe don't do any checks
  if (!std::is_same<T, U>::value) {
    throw std::logic_error("Compile type type check failed");
  }
}
}  // namespace capy

#endif  // GENERAL_TPP

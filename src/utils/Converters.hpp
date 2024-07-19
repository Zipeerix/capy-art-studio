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

#ifndef CONVERTERS_HPP
#define CONVERTERS_HPP

#include <cstdint>

namespace capy::utils::converters {
enum class StorageSize {
  Bytes,
  Kilobytes,
  Megabytes,
};

enum class TimeType {
  Hours,
  Minutes,
  Seconds,
  Miliseconds,
  Microseconds,
};

// TODO: Two way enum conversions
double convertBytesTo(uint64_t bytes, StorageSize targetType);
// TODO: std::chrono? time_cast
uint64_t convertSecondsTo(uint64_t seconds, TimeType targetType);
}  // namespace capy::utils::converters

#endif  // CONVERTERS_HPP

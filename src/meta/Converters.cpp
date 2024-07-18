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

#include "Converters.hpp"

#include <stdexcept>

namespace capy {
double convertBytesTo(const uint64_t bytes, const StorageSize targetType) {
  switch (targetType) {
    case StorageSize::Bytes:
      return static_cast<double>(bytes);

    case StorageSize::Kilobytes:
      return static_cast<double>(bytes) / 1024.0;

    case StorageSize::Megabytes:
      return static_cast<double>(bytes) / (1024.0 * 1024.0);  // TODO: pow

    default:
      throw std::logic_error("Invalid storage size");
  }
}

uint64_t convertSecondsTo(const uint64_t seconds, const TimeType targetType) {
  switch (targetType) {
    case TimeType::Hours:
      return seconds / 3600;

    case TimeType::Minutes:
      return seconds / 60;

    case TimeType::Seconds:
      return seconds;

    case TimeType::Miliseconds:
      return seconds * 1000;

    case TimeType::Microseconds:
      return seconds * 1000 * 1000;  // TOOD: pow, overall refactor both

    default:
      throw std::logic_error("Invalid time type");
  }
}
}  // namespace capy
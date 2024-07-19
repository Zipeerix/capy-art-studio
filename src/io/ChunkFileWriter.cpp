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

#include "ChunkFileWriter.hpp"

#include <fmt/format.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "ApplicationFilesystem.hpp"
#include "meta/UnimplementedException.hpp"
#include "utils/General.hpp"

namespace capy {
PotentialError<std::string> ChunkFileWriter::existingFileStrategyCheck(
    const std::string& path, const ExistingFileStrategy existingFileStrategy) {
  switch (existingFileStrategy) {
    case ExistingFileStrategy::ClearAndWrite:
    case ExistingFileStrategy::Overwrite:
      throw UnimplementedException("This file strategy is not implemented");

    case ExistingFileStrategy::Error: {
      if (std::filesystem::exists(path)) {
        return fmt::format("Unable to open file at path: {} as it already exists", path);
      }

      if (!createFileIfItDoesntExist(path)) {
        return fmt::format("Unable to create file at path: {}", path);
      }

      break;
    }

    default:
      throw std::logic_error("Invalid strategy");
  }

  return std::nullopt;
}

ChunkFileWriter::ChunkFileWriter(const std::string& path)
    : _fileStream(std::make_shared<std::ofstream>(path, std::ios::binary)) {}

ChunkFileWriter::~ChunkFileWriter() { _fileStream->close(); }

bool ChunkFileWriter::isFileValid() const { return _fileStream->good(); }

PotentialError<std::string> ChunkFileWriter::writeVector(const std::vector<uint8_t>& bytes) const {
  _fileStream->write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
  if (_fileStream->fail()) {
    return fmt::format("Failure to write to file stream");
  }

  return std::nullopt;
}

PotentialError<std::string> ChunkFileWriter::writeByte(const uint8_t byte) const {
  _fileStream->write(reinterpret_cast<const char*>(&byte), sizeof(uint8_t));
  if (_fileStream->fail()) {
    return fmt::format("Failure to write to file stream");
  }

  return std::nullopt;
}

PotentialError<std::string> ChunkFileWriter::writeQByteArray(const QByteArray& bytes) const {
  _fileStream->write(bytes.data(), bytes.size());
  if (_fileStream->fail()) {
    return fmt::format("Failure to write to file stream");
  }

  return std::nullopt;
}

PotentialError<std::string> ChunkFileWriter::writeString(const std::string& value,
                                                         const bool addNullTerminator) const {
  _fileStream->write(value.c_str(), value.size());
  if (_fileStream->fail()) {
    return fmt::format("Failure to write to file stream");
  }

  if (addNullTerminator) {
    _fileStream->write(&utils::constants::nullTerminator, 1);
    if (_fileStream->fail()) {
      return fmt::format("Failure to write to file stream");
    }
  }

  return std::nullopt;
}

PotentialError<std::string> ChunkFileWriter::write32BitInt(const uint32_t value,
                                                           const bool bigEndian) const {
  std::array<uint8_t, 4> bytes{};
  if (bigEndian) {
    bytes[0] = static_cast<uint8_t>((value >> 24) & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[3] = static_cast<uint8_t>(value & 0xFF);
  } else {
    bytes[0] = static_cast<uint8_t>(value & 0xFF);
    bytes[1] = static_cast<uint8_t>((value >> 8) & 0xFF);
    bytes[2] = static_cast<uint8_t>((value >> 16) & 0xFF);
    bytes[3] = static_cast<uint8_t>((value >> 24) & 0xFF);
  }

  _fileStream->write(reinterpret_cast<const char*>(bytes.data()), 4);
  if (_fileStream->fail()) {
    return fmt::format("Failure to write to file stream");
  }

  return std::nullopt;
}
}  // namespace capy

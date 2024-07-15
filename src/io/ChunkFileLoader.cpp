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

#include "ChunkFileLoader.hpp"

#include <fmt/format.h>

#include <stdexcept>

namespace capy {
ChunkFileLoader::ChunkFileLoader(const std::string& path) : _fileStream(path, std::ios::binary) {}

ChunkFileLoader::~ChunkFileLoader() { _fileStream.close(); }

bool ChunkFileLoader::isFileValid() const { return _fileStream.good(); }

std::size_t ChunkFileLoader::currentReadingIndex() { return _fileStream.tellg(); }

void ChunkFileLoader::setReadingIndex(std::size_t index) { _fileStream.seekg(index); }

Result<std::vector<uint8_t>, std::string> ChunkFileLoader::readNextBytesToVector(const int nBytes) {
  std::vector<uint8_t> vecBuffer(nBytes, 0);

  _fileStream.read(reinterpret_cast<char*>(vecBuffer.data()), nBytes);
  if (_fileStream.gcount() != nBytes) {
    return std::unexpected(fmt::format("Unable to read further {} bytes", nBytes));
  }

  return vecBuffer;
}

Result<uint8_t, std::string> ChunkFileLoader::readNextByte() {
  uint8_t data = 0x00;

  _fileStream.read(reinterpret_cast<char*>(&data), 1);
  if (_fileStream.gcount() != 1) {
    return std::unexpected("Unable to read further 1 bytes");
  }

  return data;
}

Result<QByteArray, std::string> ChunkFileLoader::readNextBytesToQByteArray(const int nBytes) {
  // TODO: Combine this and to vector since api is the same
  QByteArray qBuffer(nBytes, 0);

  _fileStream.read(qBuffer.data(), nBytes);
  if (_fileStream.gcount() != nBytes) {
    return std::unexpected(fmt::format("Unable to read further {} bytes", nBytes));
  }

  return qBuffer;
}

Result<std::string, std::string> ChunkFileLoader::readNextString(const int size,
                                                                 const bool nullTerminated) {
  if (size < 0) {
    throw std::invalid_argument("Invalid size argument for string reading");
  }

  if (size == 0) {
    return "";
  }

  const auto rawCharsRes = readNextBytesToVector(size);
  if (!rawCharsRes.has_value()) {
    return std::unexpected(rawCharsRes.error());
  }

  const auto& rawChars = rawCharsRes.value();

  return std::string{rawChars.begin(), nullTerminated ? rawChars.end() - 1 : rawChars.end()};
}

Result<std::string, std::string> ChunkFileLoader::readNextVariableLengthString() {
  std::string buffer;
  while (true) {
    const auto nextByteReadRes = readNextByte();
    if (!nextByteReadRes.has_value()) {
      return std::unexpected("Unexpected end of file before reaching null terminator");
    }

    const auto nextByte = nextByteReadRes.value();
    if (nextByte == 0x00) {
      return buffer;
    }

    buffer.push_back(static_cast<char>(nextByte));
  }
}

Result<std::uint32_t, std::string> ChunkFileLoader::readNext32BitInt(const bool bigEndian) {
  const auto bytesRes = readNextBytesToVector(sizeof(std::uint32_t));
  if (!bytesRes.has_value()) {
    return std::unexpected(bytesRes.error());
  }

  const auto& bytes = bytesRes.value();

  // TODO: Refactor below
  uint32_t value;
  if (bigEndian) {
    value = (static_cast<uint32_t>(static_cast<uint8_t>(bytes[3])) |
             (static_cast<uint32_t>(static_cast<uint8_t>(bytes[2])) << 8) |
             (static_cast<uint32_t>(static_cast<uint8_t>(bytes[1])) << 16) |
             (static_cast<uint32_t>(static_cast<uint8_t>(bytes[0])) << 24));
  } else {
    value = (static_cast<uint32_t>(static_cast<uint8_t>(bytes[0])) |
             (static_cast<uint32_t>(static_cast<uint8_t>(bytes[1])) << 8) |
             (static_cast<uint32_t>(static_cast<uint8_t>(bytes[2])) << 16) |
             (static_cast<uint32_t>(static_cast<uint8_t>(bytes[3])) << 24));
  }

  return value;
}

void ChunkFileLoader::moveIteratorBackBy(int offset) {
  const auto current = currentReadingIndex();
  setReadingIndex(current - offset);
}
}  // namespace capy

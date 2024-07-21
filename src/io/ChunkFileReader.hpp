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

#ifndef CHUNKFILELOADER_HPP
#define CHUNKFILELOADER_HPP

#include <cstdint>
#include <fstream>
#include <QByteArray>
#include <vector>

#include "meta/ErrorHandling.hpp"

namespace capy
{
class ChunkFileReader
{
public:
  explicit ChunkFileReader(const std::string& path);
  ~ChunkFileReader();

  bool isFileValid() const;

  std::streamoff currentReadingIndex();
  void setReadingIndex(std::streamoff index);

  // TODO: also to buffer, usefull for large reads (or is move enough)
  [[nodiscard]] Result<std::vector<uint8_t>, std::string> readNextBytesToVector(int nBytes);
  [[nodiscard]] Result<uint8_t, std::string> readNextByte();
  [[nodiscard]] Result<QByteArray, std::string> readNextBytesToQByteArray(int nBytes);
  [[nodiscard]] Result<std::string, std::string> readNextString(int size, bool nullTerminated);
  [[nodiscard]] Result<std::string, std::string> readNextVariableLengthString();

  // TODO: Make this more generic, read below
  [[nodiscard]] Result<std::uint32_t, std::string> readNext32BitInt(bool bigEndian = false);

  // TODO: Templated read

private:
  std::ifstream _fileStream;

  void moveIteratorBackBy(int offset);
};
} // namespace capy

#endif // CHUNKFILELOADER_HPP

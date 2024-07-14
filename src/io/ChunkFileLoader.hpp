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

#include <QByteArray>
#include <cstdint>
#include <fstream>
#include <vector>

#include "utils/ErrorHandling.hpp"

namespace capy {
class ChunkFileLoader {
 public:
  explicit ChunkFileLoader(const std::string& path);
  ~ChunkFileLoader();

  bool isFileValid() const;

  std::size_t currentReadingIndex();
  void setReadingIndex(std::size_t index);

  // TODO: also to buffer, usefull for large reads (or is move enough)
  Result<std::vector<uint8_t>, std::string> readNextBytesToVector(int nBytes);
  Result<uint8_t, std::string> readNextByte();
  Result<QByteArray, std::string> readNextBytesToQByteArray(int nBytes);
  Result<std::string, std::string> readNextString(int size, bool nullTerminated);
  Result<std::string, std::string> readNextVariableLengthString();

  // TODO: Make this more generic, read below
  Result<std::uint32_t, std::string> readNext32BitInt(bool bigEndian = false);

  // TODO: Templated read

 private:
  std::ifstream _fileStream;

  void moveIteratorBackBy(int offset);
};
}  // namespace capy

#endif  // CHUNKFILELOADER_HPP

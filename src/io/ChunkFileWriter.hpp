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

#ifndef CHUNKFILEWRITER_HPP
#define CHUNKFILEWRITER_HPP

#include <fstream>
#include <QByteArray>

#include "meta/ErrorHandling.hpp"

namespace capy
{
class ChunkFileWriter
{
public:
  enum class ExistingFileStrategy
  {
    ClearAndWrite,
    Overwrite,
    Error,
  };

  [[nodiscard]] static PotentialError<std::string>
  existingFileStrategyCheck(const std::string& path, ExistingFileStrategy existingFileStrategy);

  explicit ChunkFileWriter(const std::string& path);
  ~ChunkFileWriter();

  bool isFileValid() const;

  [[nodiscard]] PotentialError<std::string> writeVector(const std::vector<uint8_t>& bytes) const;
  [[nodiscard]] PotentialError<std::string> writeByte(uint8_t byte) const;
  [[nodiscard]] PotentialError<std::string> writeQByteArray(const QByteArray& bytes) const;
  [[nodiscard]] PotentialError<std::string> writeString(const std::string& value,
                                                        bool addNullTerminator) const;
  [[nodiscard]] PotentialError<std::string> write32BitInt(uint32_t value,
                                                          bool bigEndian = false) const;

private:
  // TODO: Rrevert back to value not pointer
  std::shared_ptr<std::ofstream> _fileStream = nullptr;

  // TODO: ChunkFileWriter.tpp, template <typename StlIntefaceType> writeStlType and use in all
  // data() .size() same for reader
};
} // namespace capy

#endif // CHUNKFILEWRITER_HPP

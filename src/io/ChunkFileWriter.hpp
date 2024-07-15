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

#include "ChunkFileManagerBase.hpp"

namespace capy {
class ChunkFileWriter : public ChunkFileManagerBase {
 public:
  enum class ExisingFileStrategy {
    ClearAndWrite,
    Overwrite,
    Error,
  };

  explicit ChunkFileWriter(const std::string& path, ExisingFileStrategy existingFileStrategy);

 private:
  ExisingFileStrategy _existingFileStrategy;

  void initStrategy();
};
}  // namespace capy

#endif  // CHUNKFILEWRITER_HPP

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

#ifndef CHUNKFILEMANAGERBASE_HPP
#define CHUNKFILEMANAGERBASE_HPP

#include <fstream>

namespace capy {
class ChunkFileManagerBase {
 public:
  explicit ChunkFileManagerBase(const std::string& path);
  ~ChunkFileManagerBase();

  bool isFileValid() const;

  std::size_t currentReadingIndex();
  void setReadingIndex(std::size_t index);

 protected:
  std::ifstream _fileStream;

  void moveIteratorBackBy(int offset);
};
}  // namespace capy

#endif  // CHUNKFILEMANAGERBASE_HPP

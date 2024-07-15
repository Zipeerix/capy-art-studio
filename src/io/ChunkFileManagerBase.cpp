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

#include "ChunkFileManagerBase.hpp"

namespace capy {
ChunkFileManagerBase::ChunkFileManagerBase(const std::string& path)
    : _fileStream(path, std::ios::binary) {}

ChunkFileManagerBase::~ChunkFileManagerBase() { _fileStream.close(); }

bool ChunkFileManagerBase::isFileValid() const { return _fileStream.good(); }

std::size_t ChunkFileManagerBase::currentReadingIndex() { return _fileStream.tellg(); }

void ChunkFileManagerBase::setReadingIndex(std::size_t index) { _fileStream.seekg(index); }

void ChunkFileManagerBase::moveIteratorBackBy(int offset) {
  const auto current = currentReadingIndex();
  setReadingIndex(current - offset);
}
}  // namespace capy

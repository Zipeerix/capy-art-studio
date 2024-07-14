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

#include "Project.hpp"

#include <fmt/format.h>

#include <QDebug>
#include <fstream>
#include <utility>

#include "io/ApplicationFilesystem.hpp"
#include "io/ConsoleLogger.hpp"

namespace capy {
Result<Project, std::string> Project::createFromFile(const std::string& path) {
  if (path.empty()) {
    return std::unexpected("File path is empty");
  }

  // TODO: Move to ChunkFileReader util class and return vector of bytes or uint etc instead of
  // buffers
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    return std::unexpected(fmt::format("Unable to open project file at {}", path));
  }

  char magicStringBuffer[5];
  file.read(magicStringBuffer, 5);
  if (file.gcount() != 5) {
    logger::error("Error when reading project's magic string", logger::Severity::Mild);
    return std::unexpected(fmt::format("Error when reading project file at {}", path));
  }

  const std::string magicString = magicStringBuffer;
  if (magicString != "CAPY") {
    logger::error(
        fmt::format("Magic string mismatch when loading project: {} != CAPY", magicString),
        logger::Severity::Mild);
    return std::unexpected(fmt::format("Project file at {} is corrupted", path));
  }

  char miniatureSizeBuffer[4];
  file.read(miniatureSizeBuffer, 4);
  if (file.gcount() != 4) {
    logger::error("Error when reading project's miniature size", logger::Severity::Mild);
    return std::unexpected(fmt::format("Error when reading project file at {}", path));
  }

  uint32_t miniatureSize =
      (static_cast<uint32_t>(static_cast<uint8_t>(miniatureSizeBuffer[0])) |
       (static_cast<uint32_t>(static_cast<uint8_t>(miniatureSizeBuffer[1])) << 8) |
       (static_cast<uint32_t>(static_cast<uint8_t>(miniatureSizeBuffer[2])) << 16) |
       (static_cast<uint32_t>(static_cast<uint8_t>(miniatureSizeBuffer[3])) << 24));

  logger::info(fmt::format("Loaded miniature with size: {}", miniatureSize));

  qDebug() << "Miniature size: " << miniatureSize;

  char* miniatureBytes = new char[miniatureSize];
  file.read(miniatureBytes, miniatureSize);
  if (file.gcount() != miniatureSize) {
    logger::error("Error when reading project's miniature data", logger::Severity::Mild);
    return std::unexpected(fmt::format("Error when reading project file at {}", path));
  }

  const QByteArray miniatureQByteArray{miniatureBytes, miniatureSize};
  QPixmap miniature;
  if (!miniature.loadFromData(miniatureQByteArray)) {
    logger::error("Error when creating miniature pixmap from data", logger::Severity::Mild);
  }

  delete[] miniatureBytes;

  // TODO: load layer data

  file.close();

  return Project(path, miniature);
}

Project::Project(std::string path, QPixmap miniature)
    : _path(std::move(path)), _miniature(std::move(miniature)) {}

std::string Project::getPath() const { return _path; }

std::string Project::getName() const { return getFileNameFromPath(_path); }

QPixmap Project::getMiniature() const { return _miniature; }
}  // namespace capy
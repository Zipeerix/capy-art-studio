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
#include "io/ChunkFileLoader.hpp"
#include "io/ConsoleLogger.hpp"

namespace capy {
Result<Project, std::string> Project::createFromFile(const std::string& path) {
  if (path.empty()) {
    return std::unexpected("File path is empty");
  }

  // TODO: Break down into static methods, or just statics

  ChunkFileLoader file{path};
  if (!file.isFileValid()) {
    return std::unexpected(fmt::format("Unable to open project file at: {}", path));
  }

  const auto magicStringReadRes = file.readNextString(5, true);
  if (!magicStringReadRes.has_value()) {
    logger::error("Unable to read magic string from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  if (magicStringReadRes.value() != "CAPY") {
    logger::error(
        fmt::format("Project's magic string mismatch {} != CAPY", magicStringReadRes.value()),
        logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  const auto miniatureSizeReadRes = file.readNext32BitInt();
  if (!miniatureSizeReadRes.has_value()) {
    logger::error("Unable to read miniature size from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  std::uint32_t miniatureSize = miniatureSizeReadRes.value();
  if (miniatureSize >= INT32_MAX) {
    logger::error("Miniature size > INT32_MAX", logger::Severity::Severe);
    return std::unexpected("Project file is corrupted");
  }

  const auto miniatureDataReadRes =
      file.readNextBytesToQByteArray(static_cast<int32_t>(miniatureSize));
  if (!miniatureDataReadRes.has_value()) {
    logger::error("Unable to read miniature data from project file", logger::Severity::Mild);
    return std::unexpected("Project file is corrupted or incompatible with this version");
  }

  QPixmap miniature;
  if (!miniature.loadFromData(miniatureDataReadRes.value())) {
    logger::warning("Error when creating miniature pixmap from data", logger::Severity::Mild);
  }

  // TODO: May

  return Project(path, miniature, file.currentReadIndex());
}

Project::Project(std::string path, QPixmap miniature, const std::size_t indexOfDataAfterMiniature)
    : _path(std::move(path)),
      _miniature(std::move(miniature)),
      _indexOfDataAfterMiniature(indexOfDataAfterMiniature) {}

std::string Project::getPath() const { return _path; }

std::string Project::getName() const { return getFileNameFromPath(_path); }

QPixmap Project::getMiniature() const { return _miniature; }
}  // namespace capy
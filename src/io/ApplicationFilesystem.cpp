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

#include "ApplicationFilesystem.hpp"

#include <fmt/format.h>

#include <QStandardPaths>
#include <filesystem>
#include <string>

#include "io/ConsoleLogger.hpp"

namespace capy {
static std::filesystem::path getApplicationFilesystemBasePath() {
  const auto path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  return std::filesystem::path(path.toStdString()) / "capy-art-studio";
}

void initApplicationFilesystem() {
  for (int i = 0; i < static_cast<int>(FilesystemPath::PathCount); i++) {
    const auto path = getFilesystemPath(static_cast<FilesystemPath>(i));
    logger::info(fmt::format("Creating folder if it doesnt exist: {}", path));
    std::filesystem::create_directories(path);
  }
}

std::string getFilesystemPath(const FilesystemPath of) {
  const auto basePath = getApplicationFilesystemBasePath();
  switch (of) {
    case FilesystemPath::Palettes: {
      return basePath / "palettes";
    }

    case FilesystemPath::PathCount:
    default:
      throw std::logic_error("Invalid option for getting filesystem path");
  }
}

std::vector<std::string> listFilesInPath(const FilesystemPath applicationPath) {
  const auto path = getFilesystemPath(applicationPath);
  std::vector<std::string> filePaths;

  for (const auto& entry : std::filesystem::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      filePaths.push_back(entry.path().string());
    }
  }

  return filePaths;
}
}  // namespace capy
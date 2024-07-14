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

#include <utility>

#include "io/ApplicationFilesystem.hpp"

namespace capy {
Project Project::createFromPathWithoutLoading(const std::string& path) {
  Project project{};
  project.setPath(path);

  return project;
}

std::string Project::getName() const {
  const auto path = getPath();
  if (!path.has_value()) {
    return "Unknown";
  }

  return getFileNameFromPath(path.value());
}

PotentialError<std::string> Project::importValuesFromJson(
    [[maybe_unused]] const rapidjson::Document& root) {
  return std::nullopt;
}

rapidjson::Document Project::exportValuesToJson() const {
  rapidjson::Document document;
  document.SetObject();
  return document;
}
}  // namespace capy
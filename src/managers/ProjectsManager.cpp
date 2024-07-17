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

#include "ProjectsManager.hpp"

#include <fmt/format.h>

#include <filesystem>

#include "io/ApplicationFilesystem.hpp"
#include "io/ConsoleLogger.hpp"
#include "meta/ErrorHandling.hpp"

namespace capy {
ProjectsManager::ProjectsManager(QObject* parent)
    : QObject(parent),
      _configurationManager(ConfigurationManager::createInstance()),
      _model(this) {}

bool ProjectsManager::isProjectInternal(const Project& project) const {
  return !_configurationManager->doesAdditionalProjectExist(project.getPath());
}

const std::vector<Project>& ProjectsManager::getProjects() const { return _model.getProjects(); }

void ProjectsManager::loadProjectsFromFilesystem() {
  std::vector<Project> projects;

  auto projectsPaths = listFilesInPath(FilesystemPath::Projects);
  const auto additionalProjectsPathsList = _configurationManager->getAdditionalProjectsPaths();

  projectsPaths.insert(projectsPaths.end(), additionalProjectsPathsList.begin(),
                       additionalProjectsPathsList.end());

  // TODO: std::transform
  for (const auto& path : projectsPaths) {
    logger::info(fmt::format("Adding project from path {}", path));
    const auto projectCreationResult = Project::createFromFile(path);
    if (!projectCreationResult.has_value()) {
      // TODO: This should probably be a messagebox, also anotyher problem is when "open" is clicked
      // TODO: and this fails then the list will not contain this project
      // TODO: Also it should probably be deleted from projectsPaths if its corrupted
      const auto& error = projectCreationResult.error();
      logger::error(fmt::format("Unable to load project from {} due to error: {}", path, error),
                    logger::Severity::Severe);
      continue;
    }

    projects.push_back(projectCreationResult.value());
  }

  _model.setProjects(projects);

  emit projectsUpdated();
}

void ProjectsManager::addProject(const std::string& path) {
  _configurationManager->addAdditionalProjectPath(path);
  loadProjectsFromFilesystem();
}

void ProjectsManager::deleteProject(const std::string& projectPath,
                                    const ManagerErrorHandler& errorHandler) {
  removeProject(projectPath);

  PotentialError<std::string> fsError;
  try {
    if (!std::filesystem::remove(projectPath)) {
      fsError = "File not found";
    }
  } catch (const std::filesystem::filesystem_error& err) {
    fsError = err.what();
  }

  if (fsError.has_value()) {
    return errorHandler(
        fmt::format("Unable to delete profile file due to error: {}", fsError.value()));
  }

  logger::info(fmt::format("Deleted project file at {}", projectPath));

  loadProjectsFromFilesystem();
}

void ProjectsManager::removeProject(const std::string& projectPath) {
  _configurationManager->removeAdditionalProjectPath(projectPath);
  loadProjectsFromFilesystem();
}
}  // namespace capy

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

#ifndef PROJECTSMANAGER_HPP
#define PROJECTSMANAGER_HPP

#include "io/ConfigurationManager.hpp"
#include "Manager.hpp"
#include "models/ProjectsModel.hpp"

namespace capy
{
class ProjectsManager final : public QObject
{
  Q_OBJECT
public:
  explicit ProjectsManager(QObject* parent);

  // TODO: base class Manager and also dont pass throguh model fns just have model() method
  // same as in palettesmanager
  const std::vector<Project>& getProjects() const;

  bool isProjectInternal(const Project& project) const;

  void loadProjectsFromFilesystem();

  void addProject(const std::string& path);
  void removeProject(const std::string& projectPath);
  void deleteProject(const std::string& projectPath, const ManagerErrorHandler& errorHandler);

signals:
  void projectsUpdated();

private:
  std::shared_ptr<ConfigurationManager> _configurationManager;
  models::ProjectsModel _model;
};
} // namespace capy

#endif // PROJECTSMANAGER_HPP

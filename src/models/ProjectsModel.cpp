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

#include "ProjectsModel.hpp"

namespace capy::models {
ProjectsModel::ProjectsModel(QObject* parent) : QAbstractListModel(parent) {}

bool ProjectsModel::doesProjectExist(const std::string& name) const {
  return std::ranges::any_of(_projects,
                             [name](const auto& project) { return project.getName() == name; });
}

void ProjectsModel::setProjects(std::vector<Project> projects) {
  beginResetModel();
  _projects = std::move(projects);
  endResetModel();
}

const Project& ProjectsModel::getProject(const int index) const { return _projects.at(index); }

const std::vector<Project>& ProjectsModel::getProjects() const { return _projects; }

int ProjectsModel::rowCount([[maybe_unused]] const QModelIndex& parent) const {
  return _projects.size();
}

QVariant ProjectsModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || isRowOutsideModel(index)) {
    return QVariant();
  }

  const auto& project = _projects.at(index.row());
  switch (role) {
    case Qt::DisplayRole:
      return QString::fromStdString(project.getName());

    default:
      return QVariant();
  }
}

bool ProjectsModel::isRowOutsideModel(const QModelIndex& index) const {
  return isRowOutsideModel(index.row());
}

bool ProjectsModel::isRowOutsideModel(const int index) const {
  return index < 0 || static_cast<size_t>(index) >= _projects.size();
}
}  // namespace capy::models
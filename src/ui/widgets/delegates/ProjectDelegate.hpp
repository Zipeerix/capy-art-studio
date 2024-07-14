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

#ifndef CAPY_UI_PROJECTDELEGATE_HPP
#define CAPY_UI_PROJECTDELEGATE_HPP

#include <QWidget>

#include "user/Project.hpp"

namespace capy::ui {
namespace Ui {
class ProjectDelegate;
}

// TODO: pass model and index? or find another way to use model in list or get rid of model or do
// nothing :D
class ProjectDelegate final : public QWidget {
  Q_OBJECT
 public:
  explicit ProjectDelegate(Project project, bool isProjectInternal, QWidget* parent = nullptr);
  ~ProjectDelegate() override;

 public slots:
  void imageOrNameClicked();

 signals:
  void itemClicked(const std::optional<std::string>& projectPath);
  void removeClicked(const std::optional<std::string>& projectPath);
  void deleteClicked(const std::optional<std::string>& projectPath);

 private:
  Ui::ProjectDelegate* ui;
  Project _project;
};

}  // namespace capy::ui

#endif  // CAPY_UI_PROJECTDELEGATE_HPP

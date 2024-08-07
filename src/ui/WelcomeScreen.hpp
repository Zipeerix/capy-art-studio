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

#ifndef CAPY_UI_WELCOMESCREEN_HPP
#define CAPY_UI_WELCOMESCREEN_HPP

#include <QGridLayout>
#include <QMainWindow>
#include <QTimer>

#include "managers/ProjectsManager.hpp"
#include "models/ProjectsModel.hpp"
#include "ui/layouts/FlowLayout.hpp"
#include "ui/MainWindow.hpp"

namespace capy::ui
{
namespace Ui
{
class WelcomeScreen;
}

class WelcomeScreen final : public QMainWindow, AutoSizeSavingItem
{
  Q_OBJECT
public:
  explicit WelcomeScreen(MainWindow* mainWindow, QWidget* parent = nullptr);
  ~WelcomeScreen() override;

  void resizeEvent(QResizeEvent* event) override;

public slots:
  void createNewProjectClicked();
  void openProjectClicked();
  void importProjectClicked();
  void settingsClicked();
  void continueButtonClicked();

  // TODO: When fully implemented the path shouldn't be optional
  void projectClicked(const Project& project);
  void projectRemoveClicked(const Project& project);
  void projectDeleteClicked(const Project& project);

private:
  Ui::WelcomeScreen* ui;
  std::shared_ptr<ConfigurationManager> _configurationManager;
  ProjectsManager _projectsManager;
  MainWindow* _mainWindow;
  FlowLayout* _projectAreaLayout;

  int _currentColumns = 0;

  void closeEvent(QCloseEvent* event) override;
  void updateUiProjectList();
};
} // namespace capy::ui

#endif // CAPY_UI_WELCOMESCREEN_HPP

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

#include "WelcomeScreen.hpp"

#include <QFileDialog>
#include <QMessageBox>

#include "MainWindow.hpp"
#include "io/ApplicationFilesystem.hpp"
#include "io/ConsoleLogger.hpp"
#include "ui/SettingsDialog.hpp"
#include "ui/widgets/delegates/ProjectDelegate.hpp"
#include "ui/widgets/utils/MessageBoxUtils.hpp"
#include "ui_WelcomeScreen.h"
#include "utils/UiHelpers.hpp"

namespace capy::ui {
WelcomeScreen::WelcomeScreen(MainWindow* mainWindow, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::WelcomeScreen),
      _configurationManager(ConfigurationManager::createInstance()),
      _projectsManager(this),
      _mainWindow(mainWindow) {
  ui->setupUi(this);

  connect(ui->createProjectButton, &QPushButton::clicked, this,
          &WelcomeScreen::createNewProjectClicked);
  connect(ui->openProjectButton, &QPushButton::clicked, this, &WelcomeScreen::openProjectClicked);
  connect(ui->importProjectButton, &QPushButton::clicked, this,
          &WelcomeScreen::importProjectClicked);
  connect(ui->settingsButton, &QPushButton::clicked, this, &WelcomeScreen::settingsClicked);
  connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::exit);
  connect(ui->continueButton, &QPushButton::clicked, this, &WelcomeScreen::continueButtonClicked);

  _projectAreaLayout = new FlowLayout(ui->scrollAreaWidgetContents);
  ui->scrollAreaWidgetContents->setLayout(_projectAreaLayout);

  connect(&_projectsManager, &ProjectsManager::projectsUpdated, this,
          &WelcomeScreen::updateUiProjectList);

  _projectsManager.loadProjectsFromFilesystem();
}

WelcomeScreen::~WelcomeScreen() { delete ui; }

void WelcomeScreen::resizeEvent(QResizeEvent* event) {
  updateUiProjectList();
  QWidget::resizeEvent(event);
}

void WelcomeScreen::updateUiProjectList() {
  clearLayout(_projectAreaLayout);

  for (const auto& project : _projectsManager.getProjects()) {
    const bool isProjectInternal = _projectsManager.isProjectInternal(project);
    auto* delegate = new ProjectDelegate(project, isProjectInternal, this);
    connect(delegate, &ProjectDelegate::itemClicked, this, &WelcomeScreen::projectClicked);
    connect(delegate, &ProjectDelegate::removeClicked, this, &WelcomeScreen::projectRemoveClicked);
    connect(delegate, &ProjectDelegate::deleteClicked, this, &WelcomeScreen::projectDeleteClicked);

    _projectAreaLayout->addWidget(delegate);
  }
}

void WelcomeScreen::createNewProjectClicked() {
  // TODO Reuse new file dialog
}

void WelcomeScreen::openProjectClicked() {
  // TODO add project to list using ConfigurationManager and open it
  const QString filter = "Capy Project Files (*.capy)";
  const std::string filePath =
      QFileDialog::getOpenFileName(this, "Open Capy File", QString(), filter).toStdString();
  if (filePath.empty()) {
    return;
  }

  if (_configurationManager->doesAdditionalProjectExist(filePath)) {
    return execMessageBox(this, QMessageBox::Icon::Information,
                          "This file is already in your project list");
  }

  _projectsManager.addProject(filePath);

  // TODO: Open project that was loaded
}

void WelcomeScreen::importProjectClicked() {
  // TODO: Import a png/jpg and convert to capy
}

void WelcomeScreen::continueButtonClicked() {
  _mainWindow->show();
  this->hide();
}

void WelcomeScreen::settingsClicked() {
  SettingsDialog settingsDialog{this};
  settingsDialog.exec();
}

void WelcomeScreen::projectClicked(const Project& project) {
  logger::info(fmt::format("Attempting to open project at {}", project.getPath()));

  // TODO: Maybe make drawing unique_ptr for memory saving
  _mainWindow->setProject(project);
  _mainWindow->show();
  hide();
}

void WelcomeScreen::projectRemoveClicked(const Project& project) {
  const auto& projectPath = project.getPath();

  logger::info(fmt::format("Attempting to remove project at {}", projectPath));

  if (!showConfirmationDialog(this,
                              "Are you sure you want to remove this project from the list?")) {
    return;
  }

  // TODO: Maybe accept project instead of path
  _projectsManager.removeProject(projectPath);
}

void WelcomeScreen::projectDeleteClicked(const Project& project) {
  const auto& projectPath = project.getPath();

  logger::info(fmt::format("Attempting to delete project at {}", projectPath));

  if (!showConfirmationDialog(this, "Are you sure you want to delete this project?")) {
    return;
  }

  // TODO: Maybe accept project instead of path
  _projectsManager.deleteProject(projectPath, [&](const std::string& error) {
    return execMessageBox(this, QMessageBox::Critical, QString::fromStdString(error));
  });
}
}  // namespace capy::ui

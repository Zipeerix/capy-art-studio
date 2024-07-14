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
#include "io/ConsoleLogger.hpp"
#include "ui/SettingsDialog.hpp"
#include "ui/widgets/delegates/ProjectDelegate.hpp"
#include "ui/widgets/utils/MessageBoxUtils.hpp"
#include "ui_WelcomeScreen.h"
#include "io/ApplicationFilesystem.hpp"

namespace capy::ui {
WelcomeScreen::WelcomeScreen(MainWindow* mainWindow, QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::WelcomeScreen),
      _configurationManager(ConfigurationManager::createInstance()),
      _mainWindow(mainWindow),
      _projectsModel(this) {
  ui->setupUi(this);

  connect(ui->createProjectButton, &QPushButton::clicked, this,
          &WelcomeScreen::createNewProjectClicked);
  connect(ui->openProjectButton, &QPushButton::clicked, this, &WelcomeScreen::openProjectClicked);
  connect(ui->importProjectButton, &QPushButton::clicked, this, &WelcomeScreen::importProjectClicked);
  connect(ui->settingsButton, &QPushButton::clicked, this, &WelcomeScreen::settingsClicked);
  connect(ui->exitButton, &QPushButton::clicked, this, &QApplication::exit);
  connect(ui->continueButton, &QPushButton::clicked, this, &WelcomeScreen::continueButtonClicked);

  _projectAreaLayout = new FlowLayout(ui->scrollAreaWidgetContents);
  ui->scrollAreaWidgetContents->setLayout(_projectAreaLayout);

  loadProjectsFromFilesystem();
}

WelcomeScreen::~WelcomeScreen() { delete ui; }

void WelcomeScreen::resizeEvent(QResizeEvent* event) {
  updateUiProjectList();
  QWidget::resizeEvent(event);
}

void WelcomeScreen::clearLayout() const {
  while (const QLayoutItem* item = _projectAreaLayout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
      widget->deleteLater();
    }
    delete item;
  }
}

void WelcomeScreen::updateUiProjectList() {
  clearLayout();

  const auto& projects = _projectsModel.getProjects();
  for (const auto& project : projects) {
    const bool isProjectInternal = !_configurationManager->doesAdditionalProjectExist(project.getPath().value_or("Unknown"));
    auto* delegate = new ProjectDelegate(project, isProjectInternal, this);
    connect(delegate, &ProjectDelegate::itemClicked, this, &WelcomeScreen::projectClicked);
    connect(delegate, &ProjectDelegate::removeClicked, this, &WelcomeScreen::projectRemoveClicked);
    connect(delegate, &ProjectDelegate::deleteClicked, this, &WelcomeScreen::projectDeleteClicked);

    _projectAreaLayout->addWidget(delegate);
  }
}

void WelcomeScreen::createNewProjectClicked() {
  // TODO Reuse new file dialog, extend it, ConfigurationManager::addProjectPath also on remove
  // removeProjectPath
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
    execMessageBox(this, QMessageBox::Icon::Information,
                   "This file is already in your project list");
  } else {
    _configurationManager->addAdditionalProjectPath(filePath);
  }

  loadProjectsFromFilesystem();

  // TODO: Open project from path: filePath
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

void WelcomeScreen::projectClicked(const std::optional<std::string>& path) {
  if (!path.has_value()) {
    logger::error("Attempt to open project without project path", logger::Severity::Severe);
    return;
  }

  const auto& projectPath = path.value();

  logger::info(fmt::format("Attempting to open project at {}", projectPath));

  // TODO: Change project to unloaded project and have UnloadedProjectModel and now here also
  // unloaded project doesnt need to be json serializable and there is prob no need for model tODO:
  // Or maybe have full project inherit from UnloadedPrject that inherist from json serializable

  // TODO: Pass data to _mainWindow here and in MainWindow convert Project to Drawing and fill
  // layers
  _mainWindow->show();
  this->hide();
}

void WelcomeScreen::projectRemoveClicked(const std::optional<std::string>& path) {
  if (!path.has_value()) {
    logger::error("Attempt to remove project without project path", logger::Severity::Severe);
    return;
  }

  const auto& projectPath = path.value();

  logger::info(fmt::format("Attempting to remove project at {}", projectPath));

  _configurationManager->removeAdditionalProjectPath(projectPath);

  loadProjectsFromFilesystem();
}

void WelcomeScreen::projectDeleteClicked(const std::optional<std::string>& path) {
  if (!path.has_value()) {
    logger::error("Attempt to delete project without project path", logger::Severity::Severe);
    return;
  }

  const auto& projectPath = path.value();

  logger::info(fmt::format("Attempting to delete project at {}", projectPath));

  _configurationManager->removeAdditionalProjectPath(projectPath);

  bool deleted = false;
  std::optional<std::string> fsError;
  try {
    deleted = std::filesystem::remove(projectPath);
    if (!deleted) {
      fsError = "File not found";
    }
  } catch (const std::filesystem::filesystem_error& err) {
    fsError = err.what();
  }

  if (!deleted) {
    const auto errorText = QString::fromStdString(fmt::format("Unable to delete project file at {} due to error: {}", projectPath,
                              fsError.value()));
    // TODO: allow std::string as arg for below
    return execMessageBox(this, QMessageBox::Icon::Critical, errorText);
  }

  logger::info(fmt::format("Deleted project file at {}", projectPath));

  loadProjectsFromFilesystem();
}

void WelcomeScreen::loadProjectsFromFilesystem() {
  std::vector<Project> projects;

  auto projectsPaths = listFilesInPath(FilesystemPath::Projects);
  const auto additionalProjectsPathsList = _configurationManager->getAdditionalProjectsPaths();

  projectsPaths.insert(projectsPaths.end(), additionalProjectsPathsList.begin(), additionalProjectsPathsList.end());

  // TODO: std::transform
  for (const auto& path : projectsPaths) {
    logger::info(fmt::format("Adding project from path {}", path));
    projects.push_back(Project::createFromPathWithoutLoading(path));
  }

  _projectsModel.setProjects(projects);

  // TODO: Do not allow 'remove' for projects from 'projects' internal directory
  updateUiProjectList();
}
}  // namespace capy::ui

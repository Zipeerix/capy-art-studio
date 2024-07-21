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

#include "MainWindow.hpp"

#include <fmt/format.h>
#include <QFileDialog>
#include <QString>
#include "dialogs/NewFileDialog.hpp"
#include "io/ConsoleLogger.hpp"
#include "ui/SettingsDialog.hpp"
#include "ui_MainWindow.h"
#include "utils/Converters.hpp"
#include "utils/MessageBoxUtils.hpp"
#include "utils/SpacerUtils.hpp"
#include "widgets/DrawingWidget.hpp"
#include "widgets/delegates/ToolButton.hpp"

namespace capy::ui {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      AutoSizeSavingItem(this, "MainWindow"),
      _configurationManager(ConfigurationManager::createInstance()),
      ui(new Ui::MainWindow),
      _drawingWidget(new DrawingWidget(this)),
      _statusBarWidget(new StatusBarWidget(this)) {
  ui->setupUi(this);

  ui->scrollAreaWidgetContents->layout()->addWidget(_drawingWidget);

  ui->statusBar->addPermanentWidget(_statusBarWidget);
  ui->statusBar->addPermanentWidget(createExpandingSpacer(this), true);

  // TODO: For now let it stay that way, the widget gets created even if disabled but timer doesnt
  // get created TOOD: Probably change it when connections are made to settings managet to update in
  // real time
  const bool showStatusBar = _configurationManager->getApplicationSetting<bool>(
      ConfigurationManager::ApplicationSettings::ShowStatusBar);
  if (showStatusBar) {
    setupStatusBarTimer();
  }

  ui->statusBar->setVisible(showStatusBar);

  connect(ui->actionFileNew, &QAction::triggered, this, &MainWindow::menuBarFileNewClicked);
  connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::menuBarFileSaveAsClicked);
  connect(ui->actionExit, &QAction::triggered, this, &QApplication::exit);
  connect(ui->actionCloseWindow, &QAction::triggered, this, &QApplication::exit);
  connect(ui->actionSettingsOpen, &QAction::triggered, this, &MainWindow::settingsOpenClicked);
  connect(ui->actionResetZoom, &QAction::triggered, this, [&]() { _drawingWidget->resetZoom(); });

  // TODO: on file->close go back to welcome screen? only if show welcome screen enabled
  // TODO: Project history list using ConfigurationManager getProjectsList addProjectsList etc

  setupColorDock();
  setupLayersDock();
  setupToolsDock();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::closeEvent(QCloseEvent* event) {
  logger::hideConsoleWindow();  // cleanup happens in Application::start
  QMainWindow::closeEvent(event);
}

void MainWindow::menuBarFileNewClicked() {
  NewFileDialog dialog(this);
  dialog.exec();
  // TODO: if exec() == accepted or something? Maybe no need for optional
  const auto newFileDialogResult = dialog.getResult();
  if (newFileDialogResult.has_value()) {
    _drawingWidget->startNewDrawing(newFileDialogResult->width, newFileDialogResult->height);
  }
}

void MainWindow::menuBarFileSaveAsClicked() {
  // TODO: This is ULTRA expensive, maybe save list of actions to cache and then save to file on
  // exit/nth minute

  const QString filter = "Capy Project Files (*.capy)";
  const std::string filePath =
      QFileDialog::getSaveFileName(this, "Save Capy File", QString(), filter).toStdString();
  if (filePath.empty()) {
    return;
  }

  const QByteArray miniatureBytes = _drawingWidget->createMiniatureBytes();
  const std::vector<Layer>& layers = _drawingWidget->getLayers();

  const auto savingError = _project->save(miniatureBytes, layers, filePath);
  if (savingError.has_value()) {
    return execMessageBox(this, QMessageBox::Icon::Critical,
                          QString::fromStdString(savingError.value()));
  }
}

void MainWindow::settingsOpenClicked() {
  SettingsDialog settingsDialog{this};
  settingsDialog.exec();
}

void MainWindow::setupColorDock() {
  _colorDockArea = new ColorArea(this);
  connect(_colorDockArea, &ColorArea::colorPickerColorChanged, this,
          &MainWindow::colorPickerColorChanged);

  ui->colorDock->setWidget(_colorDockArea);
}

void MainWindow::setupLayersDock() {
  _layersDockArea = new LayersArea(this);
  ui->layersDock->setWidget(_layersDockArea);
}

void MainWindow::setupToolsDock() {
  _toolsDockArea = new ToolsArea(this);
  ui->toolsDock->setWidget(_toolsDockArea);

  for (int i = 0; i < static_cast<int>(DrawingTool::Count); i++) {
    const auto drawingTool = static_cast<DrawingTool>(i);
    const QString buttonName = QString::fromStdString(getDrawingToolName(drawingTool));

    ToolButton* createdButon = _toolsDockArea->addToolButton(drawingTool);
    connect(createdButon, &ToolButton::clicked, this, [&, drawingTool]() {
      _drawingWidget->switchTool(drawingTool);
    });
  }
}

void MainWindow::setupStatusBarTimer() {
  const auto statusBarUpdateInterval = _configurationManager->getApplicationSetting<int>(
      ConfigurationManager::ApplicationSettings::StatusBarUpdateInterval);
  const auto updateInternalInSeconds = static_cast<int>(convertSecondsTo(
      std::max(statusBarUpdateInterval, 1), utils::converters::TimeType::Miliseconds));
  _statusBarTimer.start(updateInternalInSeconds);
  connect(&_statusBarTimer, &QTimer::timeout, this, &MainWindow::updateStatusBar);
}

void MainWindow::updateStatusBar() const { _statusBarWidget->update(_drawingWidget->getLayers()); }

void MainWindow::changeWindowTitle(const std::string& projectPath) {
  setWindowTitle("CapyArtStudio : " + QString::fromStdString(projectPath));
}

void MainWindow::colorPickerColorChanged(const QColor newColor) const {
  logger::info(fmt::format("Changing color to: ({}, {}, {} {})", newColor.red(), newColor.green(),
                           newColor.blue(), newColor.alpha()));
  _drawingWidget->handleColorPickerColorChange(newColor);
}

void MainWindow::setProject(const Project& project) {
  const auto drawingCreationRes = project.readDrawing();
  if (!drawingCreationRes.has_value()) {
    return execMessageBox(this, QMessageBox::Icon::Critical,
                          QString::fromStdString(drawingCreationRes.error()));
  }

  _project = project;
  changeWindowTitle(_project->getPath());

  _drawingWidget->setDrawing(drawingCreationRes.value());
}
}  // namespace capy::ui
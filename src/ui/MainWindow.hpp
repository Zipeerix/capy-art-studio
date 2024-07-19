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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "docks/ColorArea.hpp"
#include "docks/LayersArea.hpp"
#include "docks/ToolsArea.hpp"
#include "user/Project.hpp"
#include "utils/AutoSizeSavingItem.hpp"
#include "widgets/DrawingWidget.hpp"
#include "widgets/StatusBarWidget.hpp"

namespace capy::ui {
namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow, AutoSizeSavingItem {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

  void setProject(const Project& project);

 public slots:
  void menuBarFileNewClicked();
  void menuBarFileSaveAsClicked();
  void settingsOpenClicked();
  void colorPickerColorChanged(QColor newColor) const;

 private:
  std::shared_ptr<ConfigurationManager> _configurationManager;
  Ui::MainWindow* ui;
  DrawingWidget* _drawingWidget;
  StatusBarWidget* _statusBarWidget;

  QTimer _statusBarTimer;

  ColorArea* _colorDockArea = nullptr;
  LayersArea* _layersDockArea = nullptr;
  ToolsArea* _toolsDockArea = nullptr;

  std::optional<Project> _project = std::nullopt;

  void setupColorDock();
  void setupLayersDock();
  void setupToolsDock();
  void setupStatusBarTimer();

  void updateStatusBar() const;

  void closeEvent(QCloseEvent* event) override;

  void changeWindowTitle(const std::string& projectPath);
};
}  // namespace capy::ui

#endif  // MAINWINDOW_H

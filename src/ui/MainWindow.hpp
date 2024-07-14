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

#include "docks/ColorArea.hpp"
#include "docks/LayersArea.hpp"
#include "docks/ToolsArea.hpp"
#include "widgets/DrawingWidget.hpp"

namespace capy::ui {
namespace Ui {
class MainWindow;
}

class MainWindow final : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

 public slots:
  void menuBarFileNewClicked();
  void settingsOpenClicked();
  void colorPickerColorChanged(QColor newColor) const;

 private:
  Ui::MainWindow* ui;
  DrawingWidget* _drawingWidget;

  ColorArea* _colorDockArea = nullptr;
  LayersArea* _layersDockArea = nullptr;
  ToolsArea* _toolsDockArea = nullptr;

  void setupColorDock();
  void setupLayersDock();
  void setupToolsDock();
};
}  // namespace capy::ui

#endif  // MAINWINDOW_H

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

#include "docks/ColorPaletteArea.hpp"
#include "docks/ColorPickerArea.hpp"
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

  void colorPickerColorChanged(QColor newColor);

 private:
  Ui::MainWindow* ui;
  DrawingWidget* _drawingWidget;

  ColorPickerArea* _colorPickerDockArea;
  ColorPaletteArea* _colorPaletteDockArea;
  LayersArea* _layersDockArea;
  ToolsArea* _toolsDockArea;

  void setupColorPickerDock();
  void setupColorPaletteDock();
  void setupLayersDock();
  void setupToolsDock();
};
}  // namespace capy::ui

#endif  // MAINWINDOW_H

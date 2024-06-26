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

#include "../io/ConsoleLogger.hpp"
#include "dialogs/NewFileDialog.hpp"
#include "ui_mainwindow.h"
#include "widgets/DrawingWidget.hpp"

namespace capy::ui {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), _drawingWidget(new DrawingWidget(this)) {
  ui->setupUi(this);

  connect(ui->actionFileNew, &QAction::triggered, this, &MainWindow::menuBarFileNewClicked);

  ui->scrollAreaWidgetContents->layout()->addWidget(_drawingWidget);

  // TODO: maybe all docks should have references to Drawing and Tooldock or
  // something
  // TODO: so that singlas can be passed there instead of doing a lot on
  // MainWindow
  setupColorPickerDock();
  setupColorPaletteDock();
  setupLayersDock();
  setupToolsDock();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::menuBarFileNewClicked() {
  NewFileDialog dialog(this);
  dialog.exec();
  // TODO: if exec() == accepted or something? Maybe no need for optional
  const auto newFileDialogResult = dialog.getResult();
  if (newFileDialogResult.has_value()) {
    _drawingWidget->startNewDrawing(newFileDialogResult->width, newFileDialogResult->height);
  }
}

void MainWindow::setupColorPickerDock() {
  _colorPickerDockArea = new ColorPickerArea(this);
  connect(_colorPickerDockArea, &ColorPickerArea::colorPickerColorChanged, this,
          &MainWindow::colorPickerColorChanged);

  ui->colorPickerDock->setWidget(_colorPickerDockArea);
}

void MainWindow::setupColorPaletteDock() {
  _colorPaletteDockArea = new ColorPaletteArea(this);
  ui->colorPaletteDock->setWidget(_colorPaletteDockArea);
}

void MainWindow::setupLayersDock() {
  _layersDockArea = new LayersArea(this);
  ui->layersDock->setWidget(_layersDockArea);
}

void MainWindow::setupToolsDock() {
  _toolsDockArea = new ToolsArea(this);
  ui->toolsDock->setWidget(_toolsDockArea);
}

void MainWindow::colorPickerColorChanged(QColor newColor) {
  logger::info(fmt::format("Changing color to: ({}, {}, {} {})", newColor.red(), newColor.green(),
                           newColor.blue(), newColor.alpha()));
  _drawingWidget->setDrawingColor(newColor);
}
}  // namespace capy::ui
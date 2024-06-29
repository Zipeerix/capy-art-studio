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

#include "dialogs/NewFileDialog.hpp"
#include "ui_mainwindow.h"
#include "utils/ConsoleLogger.hpp"
#include "widgets/ColorPicker.hpp"
#include "widgets/DrawingWidget.hpp"

namespace capy::ui {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _drawingWidget(new DrawingWidget(this)) {
  ui->setupUi(this);

  setupDock(ui->toolsDock);
  setupDock(ui->colorPickerDock);
  setupDock(ui->colorPaletteDock);
  setupDock(ui->layersDock);

  ui->scrollAreaWidgetContents->layout()->addWidget(_drawingWidget);
  ui->colorPickerDock->setWidget(new ColorPicker(this));

  connect(ui->actionFileNew, &QAction::triggered, this,
          &MainWindow::menuBarFileNewClicked);
  connect(ui->layerSpinBox, &QSpinBox::valueChanged, this,
          &MainWindow::currentLayerChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupDock(QDockWidget* dockWidget) {
  const auto palette = dockWidget->palette();
  const QColor backgroundColor = palette.color(QPalette::Window);
  const QColor borderColor = palette.color(QPalette::Shadow);
  dockWidget->setStyleSheet(QString("QDockWidget::title {"
                                    "    background: %1;"
                                    "    border: 1px solid %2;"
                                    "    padding: 5px;"
                                    "}")
                                .arg(backgroundColor.name())
                                .arg(borderColor.name()));
}

void MainWindow::menuBarFileNewClicked() {
  NewFileDialog dialog(this);
  dialog.exec();
  // TODO: if exec() == accepted or something? Maybe no need for optional
  const auto newFileDialogResult = dialog.getResult();
  if (newFileDialogResult.has_value()) {
    _drawingWidget->startNewDrawing(newFileDialogResult->width,
                                    newFileDialogResult->height);
  }
}

void MainWindow::currentLayerChanged(int newLayer) {
  logger::debug(fmt::format("Layer changed to: {}", newLayer), "MainWindow");
  _drawingWidget->setCurrentLayer(newLayer);
}
}  // namespace capy::ui
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

#include "dialogs/NewFileDialog.hpp"
#include "ui_mainwindow.h"
#include "widgets/DrawingWidget.hpp"

namespace capy::ui {
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _drawingWidget(new DrawingWidget(this)) {
  ui->setupUi(this);
  ui->scrollAreaWidgetContents->layout()->addWidget(_drawingWidget);

  setupDock(ui->toolsDock);
  setupDock(ui->colorsDock);
  setupDock(ui->layersDock);

  connect(ui->actionFileNew, SIGNAL(triggered()), this,
          SLOT(menuBarFileNewClicked()));
  connect(ui->layerSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(currentLayerChanged(int)));

  connect(ui->redButton, SIGNAL(clicked()), this, SLOT(redButtonClicked()));
  connect(ui->blackButton, SIGNAL(clicked()), this, SLOT(blackButtonClicked()));
  connect(ui->opacitySpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(opacityChanged(int)));
}

void MainWindow::blackButtonClicked() {
  const auto spinBoxValue = ui->opacitySpinBox->value();
  _drawingWidget->setDrawingColor(QColor(0, 0, 0, 255));
}

void MainWindow::redButtonClicked() {
  const auto spinBoxValue = ui->opacitySpinBox->value();
  _drawingWidget->setDrawingColor(QColor(255, 0, 0, 255));
}

void MainWindow::opacityChanged(int value) {
  auto color = _drawingWidget->getDrawingColor();
  color.setAlpha(value);

  _drawingWidget->setDrawingColor(color);
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
  _drawingWidget->setCurrentLayer(newLayer);
}
}  // namespace capy::ui
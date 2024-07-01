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

#include "ConsoleWindow.hpp"

#include "ui_ConsoleWindow.h"

namespace capy::ui {
ConsoleWindow::ConsoleWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::ConsoleWindow) {
  ui->setupUi(this);
}

ConsoleWindow::~ConsoleWindow() { delete ui; }

void ConsoleWindow::log(const QString& message) const { ui->logTextArea->append(message + "\n"); }
}  // namespace capy::ui

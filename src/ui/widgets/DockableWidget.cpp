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

#include "DockableWidget.hpp"

#include <QDockWidget>

namespace capy::ui {
DockableWidget::DockableWidget(QWidget* parent) :
  QDockWidget(parent) {
  const auto palette = this->palette();
  const QColor backgroundColor = palette.color(QPalette::Window);
  const QColor borderColor = palette.color(QPalette::Shadow);
  setStyleSheet(QString("QDockWidget::title {"
                                    "    background: %1;"
                                    "    border: 1px solid %2;"
                                    "    padding: 5px;"
                                    "}")
                                .arg(backgroundColor.name())
                                .arg(borderColor.name()));
}
}
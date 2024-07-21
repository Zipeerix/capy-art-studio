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

#include <stdexcept>
#include "ToolButton.hpp"
#include "io/ResourceManager.hpp"
#include "graphics/drawing-tools/IDrawingTool.hpp"

namespace capy::ui {
ResourceManager::ToolIcon getIconForTool(const DrawingTool tool) {
  switch (tool) {
    case DrawingTool::Hand:
      return ResourceManager::ToolIcon::Hand;

    case DrawingTool::Pen:
      return ResourceManager::ToolIcon::Pen;

    case DrawingTool::Count:
    default:
      throw std::logic_error("Invalid tool");
  }
}

ToolButton::ToolButton(const DrawingTool tool, QWidget* parent) :
  QPushButton(parent) {
  setFlat(true);

  const QString iconPath = ResourceManager::getToolIconPath(getIconForTool(tool));
  setIcon(QIcon(iconPath));
  setIconSize(QSize(25, 25));

  paintAsUnclicked();
}

void ToolButton::paintAsClicked() {
  // TODO
}

void ToolButton::paintAsUnclicked() {
  // TODO
}
}
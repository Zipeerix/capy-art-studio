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

#include "ToolsArea.hpp"

#include "ui/widgets/delegates/ToolButton.hpp"
#include "ui_ToolsArea.h"

namespace capy::ui
{
ToolsArea::ToolsArea(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ToolsArea)
{
  ui->setupUi(this);

  _layout = new FlowLayout(ui->scrollArea, 0, 0, 0);
  // TODO: Set spacing in cosntructor

  ui->scrollArea->setLayout(_layout);
}

ToolsArea::~ToolsArea()
{
  delete ui;
}

ToolButton* ToolsArea::addToolButton(const DrawingTool tool)
{
  auto* toolButton = new ToolButton(tool, this);
  _layout->addWidget(toolButton);

  // TODO: Button on click hight light it, unhigh light all the others

  return toolButton;
}
} // namespace capy::ui

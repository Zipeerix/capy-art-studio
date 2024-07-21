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

#ifndef CAPY_UI_TOOLSAREA_HPP
#define CAPY_UI_TOOLSAREA_HPP

#include <QWidget>
#include <QString>
#include "ui/layouts/FlowLayout.hpp"
#include "ui/widgets/delegates/ToolButton.hpp"

namespace capy::ui {
namespace Ui {
class ToolsArea;
}

class ToolsArea final : public QWidget {
  Q_OBJECT
public:
  explicit ToolsArea(QWidget *parent = nullptr);
  ~ToolsArea() override;

  ToolButton* addToolButton(DrawingTool tool);

private:
  Ui::ToolsArea* ui;
  FlowLayout* _layout;
};
} // namespace capy::ui

#endif // CAPY_UI_TOOLSAREA_HPP

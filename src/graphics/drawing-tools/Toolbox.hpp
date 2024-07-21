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

#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <memory>
#include "HandTool.hpp"
#include "PenTool.hpp"

namespace capy {
namespace ui {
class DrawingWidget;
}

class Toolbox {
public:
  explicit Toolbox(ui::DrawingWidget* drawingWidget);

  IDrawingTool* getCurrentToolInterface() const;
  void switchTool(DrawingTool newTool);

  PenTool* getPenTool() const;
  HandTool* getHandTool() const;

private:
  ui::DrawingWidget* _drawingWidget;
  std::unique_ptr<PenTool> _penTool;
  std::unique_ptr<HandTool> _handTool;
  DrawingTool _currentTool = DrawingTool::Pen;
};
} // capy

#endif //TOOLBOX_HPP

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

#include "Toolbox.hpp"

#include <stdexcept>

#include "ui/widgets/DrawingWidget.hpp"

namespace capy
{
Toolbox::Toolbox(ui::DrawingWidget* drawingWidget) :
    _drawingWidget(drawingWidget),
    _penTool(std::make_unique<PenTool>(_drawingWidget)),
    _handTool(std::make_unique<HandTool>(_drawingWidget))
{
}

PenTool* Toolbox::getPenTool() const
{
  return _penTool.get();
}

HandTool* Toolbox::getHandTool() const
{
  return _handTool.get();
}

IDrawingTool* Toolbox::getCurrentToolInterface() const
{
  switch (_currentTool)
  {
    case DrawingTool::Hand:
      return reinterpret_cast<IDrawingTool*>(_handTool.get());

    case DrawingTool::Pen:
      return reinterpret_cast<IDrawingTool*>(_penTool.get());

    case DrawingTool::Count:
    default:
      throw std::logic_error("Invalid current tool");
  }
}

void Toolbox::switchTool(const DrawingTool newTool)
{
  if (newTool == DrawingTool::Count)
  {
    throw std::logic_error("Invalid tool");
  }

  getCurrentToolInterface()->onSwitchOutOf();
  _currentTool = newTool;
  getCurrentToolInterface()->onSwitchTo();
}
} // namespace capy
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

#include "IDrawingTool.hpp"

#include <stdexcept>

namespace capy
{
std::string getDrawingToolName(const DrawingTool drawingTool)
{
  switch (drawingTool)
  {
    case DrawingTool::Hand:
      return "Hand";

    case DrawingTool::Pen:
      return "Pen";

    case DrawingTool::Count:
    default:
      throw std::logic_error("Invalid tool");
  }
}

IDrawingTool::IDrawingTool(ui::DrawingWidget* drawingWidget) :
    _drawingWidget(drawingWidget)
{
}

ui::DrawingWidget* IDrawingTool::getDrawingWidget() const
{
  return _drawingWidget;
}
} // namespace capy
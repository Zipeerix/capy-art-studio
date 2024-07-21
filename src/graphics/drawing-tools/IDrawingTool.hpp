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

#ifndef IDRAWINGTOOL_HPP
#define IDRAWINGTOOL_HPP

#include <optional>
#include <QMouseEvent>
#include <QPoint>
#include <string>

namespace capy
{
namespace ui
{
class DrawingWidget;
}

enum class DrawingTool
{
  Hand,
  Pen,
  Count
};

std::string getDrawingToolName(DrawingTool drawingTool);

class IDrawingTool
{
public:
  explicit IDrawingTool(ui::DrawingWidget* drawingWidget);
  virtual ~IDrawingTool() = default;

  virtual void onSwitchTo() = 0;
  virtual void onSwitchOutOf() = 0;
  virtual bool mousePressEvent(QMouseEvent* event, const std::optional<QPoint>& clickedPixel) = 0;
  virtual bool mouseReleaseEvent(QMouseEvent* event, const std::optional<QPoint>& clickedPixel) = 0;
  virtual bool mouseMoveEvent(QMouseEvent* event,
                              const std::optional<QPoint>& movingThroughPixel) = 0;

protected:
  ui::DrawingWidget* _drawingWidget;
};
} // namespace capy

#endif //IDRAWINGTOOL_HPP

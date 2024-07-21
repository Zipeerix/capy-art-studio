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

#include "HandTool.hpp"

#include <QScrollBar>

#include "ui/widgets/DrawingWidget.hpp"

namespace capy
{
HandTool::HandTool(ui::DrawingWidget* drawingWidget) :
    IDrawingTool(drawingWidget)
{
}

void HandTool::onSwitchTo()
{
}

void HandTool::onSwitchOutOf()
{
}

bool HandTool::mousePressEvent(QMouseEvent* event,
                               [[maybe_unused]] const std::optional<QPoint>& clickedPixel)
{
  const auto eventPosition = event->position();
  if (event->button() == Qt::LeftButton)
  {
    _leftMouseButtonPressed = true;
    _panStartX = static_cast<int>(eventPosition.x());
    _panStartY = static_cast<int>(eventPosition.y());
    _drawingWidget->setCursor(Qt::ClosedHandCursor);
    return true;
  }

  return false;
}

bool HandTool::mouseMoveEvent(QMouseEvent* event,
                              [[maybe_unused]] const std::optional<QPoint>& movingThroughPixel)
{
  const auto eventPosition = event->position();
  if (_leftMouseButtonPressed)
  {
    _drawingWidget->horizontalScrollBar()->setValue(static_cast<int>(
            _drawingWidget->horizontalScrollBar()->value() - (eventPosition.x() - _panStartX)));
    _drawingWidget->verticalScrollBar()->setValue(static_cast<int>(
            _drawingWidget->verticalScrollBar()->value() - (eventPosition.y() - _panStartY)));
    _panStartX = static_cast<int>(eventPosition.x());
    _panStartY = static_cast<int>(eventPosition.y());
    return true;
  }

  return false;
}

bool HandTool::mouseReleaseEvent(QMouseEvent* event,
                                 [[maybe_unused]] const std::optional<QPoint>& clickedPixel)
{
  if (event->button() == Qt::LeftButton)
  {
    _leftMouseButtonPressed = false;
    _drawingWidget->setCursor(Qt::ArrowCursor);
    return true;
  }

  return false;
}
} // namespace capy
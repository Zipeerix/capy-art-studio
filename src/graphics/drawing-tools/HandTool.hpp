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

#ifndef HANDTOOL_HPP
#define HANDTOOL_HPP

#include "IDrawingTool.hpp"

namespace capy
{
class HandTool final : public IDrawingTool
{
public:
  explicit HandTool(ui::DrawingWidget* drawingWidget);
  ~HandTool() override = default;

  void onSwitchTo() override;
  void onSwitchOutOf() override;

  bool mousePressEvent(QMouseEvent* event, const std::optional<QPoint>& clickedPixel) override;
  bool mouseMoveEvent(QMouseEvent* event, const std::optional<QPoint>& movingThroughPixel) override;
  bool mouseReleaseEvent(QMouseEvent* event, const std::optional<QPoint>& clickedPixel) override;

private:
  bool _leftMouseButtonPressed = false;
  int _panStartX = 0;
  int _panStartY = 0;
};
} // namespace capy

#endif //HANDTOOL_HPP

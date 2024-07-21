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

#ifndef PENTOOL_HPP
#define PENTOOL_HPP

#include <QColor>

#include "IDrawingTool.hpp"

namespace capy
{
class PenTool final : public IDrawingTool
{
public:
  explicit PenTool(ui::DrawingWidget* drawingWidget);
  ~PenTool() override = default;

  void setColor(QColor newColor);
  QColor getColor() const;

  void onSwitchTo() override;
  void onSwitchOutOf() override;

  bool mousePressEvent(QMouseEvent* event, const std::optional<QPoint>& clickedPixel) override;
  bool mouseMoveEvent(QMouseEvent* event, const std::optional<QPoint>& movingThroughPixel) override;
  bool mouseReleaseEvent(QMouseEvent* event, const std::optional<QPoint>& clickedPixel) override;

private:
  QColor _color{0, 0, 0, 255};
  std::optional<QPoint> _lastContinousDrawingPoint = std::nullopt;
};
} // namespace capy

#endif //PENTOOL_HPP

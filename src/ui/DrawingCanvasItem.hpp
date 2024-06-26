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

#ifndef DRAWINGCANVASITEM_HPP
#define DRAWINGCANVASITEM_HPP

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QPainter>

namespace capy::ui {
class DrawingCanvasItem final : public QGraphicsItem {
public:
  DrawingCanvasItem(int width, int height);

  void updateCanvasPixel(int x, int y, const QColor& color);

private:
  QImage _canvasRepresentation;

  [[nodiscard]] QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget) override;
};
}

#endif //DRAWINGCANVASITEM_HPP

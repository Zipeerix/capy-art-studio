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


#include "DrawingCanvasItem.hpp"

namespace capy::ui {
DrawingCanvasItem::DrawingCanvasItem(int width, int height) {
  _canvasRepresentation = QImage(width, height, QImage::Format_RGBA8888);
  // TODO: How to handle this? Maybe copy from first layer at init time
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      _canvasRepresentation.setPixelColor(x, y, QColor(255, 255, 255, 255));
    }
  }
}

QRectF DrawingCanvasItem::boundingRect() const {
  return QRectF(0, 0, _canvasRepresentation.width(),
                _canvasRepresentation.height());
}

void DrawingCanvasItem::paint(QPainter* painter,
                              const QStyleOptionGraphicsItem* option,
                              QWidget* widget) {
  painter->drawImage(0, 0, _canvasRepresentation);
}

void DrawingCanvasItem::updateCanvasPixel(int x, int y, const QColor& color) {
  _canvasRepresentation.setPixelColor(x, y, color);
  update();
}
}
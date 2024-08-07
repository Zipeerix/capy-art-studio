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

#include <QBuffer>

namespace capy::ui
{
DrawingCanvasItem::DrawingCanvasItem(const int width, const int height)
{
  _canvasRepresentation = QImage(width, height, QImage::Format_RGBA8888);
  fillCanvas();
  update();
}

QRectF DrawingCanvasItem::boundingRect() const
{
  return {0, 0, static_cast<qreal>(_canvasRepresentation.width()),
          static_cast<qreal>(_canvasRepresentation.height())};
}

void DrawingCanvasItem::paint(QPainter* painter,
                              [[maybe_unused]] const QStyleOptionGraphicsItem* option,
                              [[maybe_unused]] QWidget* widget)
{
  painter->drawImage(0, 0, _canvasRepresentation);
}

void DrawingCanvasItem::updateExternalCanvasPixel(const int x, const int y, const QColor& color)
{
  _canvasRepresentation.setPixelColor(x, y, color);
  update();
}

void DrawingCanvasItem::updateAllPixels(const ColorCalculatingFunction& colorCalculatingFunction)
{
  const auto width = _canvasRepresentation.width();
  const auto height = _canvasRepresentation.height();

  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      _canvasRepresentation.setPixelColor(x, y, colorCalculatingFunction(x, y));
    }
  }

  update();
}

QByteArray DrawingCanvasItem::createMiniatureBytes() const
{
  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);

  // TODO: magic numbers
  int width = 0;
  int height = 0;
  if (_canvasRepresentation.width() > _canvasRepresentation.height())
  {
    width = 300;
    height = 200;
  }
  else if (_canvasRepresentation.width() < _canvasRepresentation.height())
  {
    width = 200;
    height = 300;
  }
  else
  {
    width = 300;
    height = 300;
  }

  const auto scaledCanvas = _canvasRepresentation.scaled(width, height);
  scaledCanvas.save(&buffer, "PNG");
  return byteArray;
}

void DrawingCanvasItem::fillCanvas()
{
  // TODO: maybe take from first layer since now they have to be SYNCED
  // TODO: and alpha = 0?
  for (int x = 0; x < _canvasRepresentation.width(); x++)
  {
    for (int y = 0; y < _canvasRepresentation.height(); y++)
    {
      _canvasRepresentation.setPixelColor(x, y, QColor(255, 255, 255, 255));
    }
  }
}
} // namespace capy::ui
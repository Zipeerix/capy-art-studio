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

#include "DrawingWidget.hpp"

#include <fmt/format.h>
#include "algorithms/Bresenham.hpp"
#include "io/ConsoleLogger.hpp"
#include "utils/General.hpp"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QScrollBar>

namespace capy::ui {
DrawingWidget::DrawingWidget(QWidget* parent) :
  QGraphicsView(parent),
  _settings(ConfigurationManager::createInstance()),
  _scene(new QGraphicsScene(this)),
  _drawing(0, 0) {
  setScene(_scene);
}

void DrawingWidget::drawBackground(QPainter* painter, [[maybe_unused]] const QRectF& rect) {
  painter->save();
  painter->resetTransform();
  // TODO: Below maybe change to rect?
  painter->drawTiledPixmap(viewport()->rect(), _checkerboardPixmap);
  painter->restore();
}

void DrawingWidget::setDrawingColor(const QColor color) {
  _drawingColor = color;
}

QColor DrawingWidget::getDrawingColor() const {
  return _drawingColor;
}

void DrawingWidget::startNewDrawing(const int width, const int height) {
  logger::info(fmt::format(
      "Creating new image with dimensions {}x{} with per layer size of {} bytes",
      width, height, calculateInMemorySizeOfImage(width, height)));
  _drawing = Drawing(width, height);
  _scene->clear();

  _drawingCanvasItem = new DrawingCanvasItem(width, height);
  _scene->addItem(_drawingCanvasItem);

  if (_settings->getGraphicsSetting<bool>(ConfigurationManager::GraphicsSetting::DrawGrid)) {
    const int totalDrawingWidth = width;
    const int totalDrawingHeight = height;

    auto pen = QPen(Qt::lightGray);
    pen.setWidthF(_settings->getGraphicsSetting<double>(ConfigurationManager::GraphicsSetting::GridWidth));
    pen.setCosmetic(true);

    for (int y = 0; y <= totalDrawingHeight; y += 1) {
      _scene->addLine(0, y, totalDrawingHeight, y, pen);
    }

    for (int x = 0; x <= totalDrawingWidth; x += 1) {
      _scene->addLine(x, 0, x, totalDrawingWidth, pen);
    }
  }
}

void DrawingWidget::setCurrentLayer(const int newLayer) {
  _drawing.setCurrentLayer(newLayer);
}

std::optional<QPoint> DrawingWidget::mapPositionOfEventToScene(
    const QMouseEvent* event) const {
  const auto mappedPosition = mapToScene(event->position().toPoint());
  const int mappedPositionX = qFloor(mappedPosition.x());
  const int mappedPositionY = qFloor(mappedPosition.y());

  // TODO: Performance optimization?
  if (mappedPositionX >= 0 &&
      mappedPositionY >= 0 &&
      mappedPositionX < _drawing.getWidth() &&
      mappedPositionY < _drawing.getHeight()) {
    return QPoint{mappedPositionX, mappedPositionY};
  }

  return std::nullopt;
}

void DrawingWidget::mousePressEvent(QMouseEvent* event) {
  const auto clickedPixel = mapPositionOfEventToScene(event);
  const QPointF eventPosition = event->position();

  // TODO: Maybe make tools their own objects and pass clickedPixel and other data to them
  switch (_tool) {
    case DrawingTool::Hand: {
      if (event->button() == Qt::LeftButton) {
        _leftMouseButtonPressed = true;
        _panStartX = static_cast<int>(eventPosition.x());
        _panStartY = static_cast<int>(eventPosition.y());
        setCursor(Qt::ClosedHandCursor);
        event->accept();
      }
      return;
    }

    case DrawingTool::Pen: {
      if (clickedPixel.has_value()) {
        const auto clickedPixelX = clickedPixel->x();
        const auto clickedPixelY = clickedPixel->y();

        _drawing.drawPixelOnCurrentLayer(clickedPixelX,
                                         clickedPixelY,
                                         _drawingColor);
        const auto combinedColor = _drawing.calculateCombinedPixelColor(
            clickedPixelX, clickedPixelY);
        _drawingCanvasItem->updateCanvasPixel(clickedPixelX, clickedPixelY,
                                                combinedColor);
      }
      return;
    }

    case DrawingTool::Eraser: // same as pen but set to invisible
      break;

    case DrawingTool::Rectangle:
      break;

    case DrawingTool::Circle:
      break;

    default:
      break;
  }

  // or maybe base to base class
  event->ignore();
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) {
  switch (_tool) {
    case DrawingTool::Hand: {
      if (event->button() == Qt::LeftButton) {
        _leftMouseButtonPressed = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
      }
      return;
    }
    case DrawingTool::Pen: {
      _lastContinousDrawingPoint = std::nullopt;
      return;
    }
    case DrawingTool::Eraser:
      break;

    case DrawingTool::Rectangle:
      break;

    case DrawingTool::Circle:
      break;

    default:
      break;
  }

  // or maybe pass to base class
  event->ignore();
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) {
  const auto movingThroughPixel = mapPositionOfEventToScene(event);
  const QPointF eventPosition = event->position();

  switch (_tool) {
    case DrawingTool::Hand: {
      if (_leftMouseButtonPressed) {
        horizontalScrollBar()->setValue(
            static_cast<int>(horizontalScrollBar()->value() - (eventPosition.x() - _panStartX)));
        verticalScrollBar()->setValue(
            static_cast<int>(verticalScrollBar()->value() - (eventPosition.y() - _panStartY)));
        _panStartX = static_cast<int>(eventPosition.x());
        _panStartY = static_cast<int>(eventPosition.y());
        event->accept();
      }
      return;
    }

    case DrawingTool::Pen: {
      if (movingThroughPixel.has_value()) {
        const auto clickedPixelX = movingThroughPixel->x();
        const auto clickedPixelY = movingThroughPixel->y();

        if (_lastContinousDrawingPoint.has_value()) {
          // TODO: Move this to a method or cleanup/tool_to_class seperation
          static const auto pixelDrawingAction = [&](int x, int y) {
            _drawing.drawPixelOnCurrentLayer(x, y, _drawingColor);
            const auto combinedColor = _drawing.calculateCombinedPixelColor(x, y);
            _drawingCanvasItem->updateCanvasPixel(x, y, combinedColor);
          };

          algorithms::applyBresenham(_lastContinousDrawingPoint->x(),
                                     _lastContinousDrawingPoint->y(),
                                     clickedPixelX,
                                     clickedPixelY,
                                     pixelDrawingAction);
        }

        _lastContinousDrawingPoint = movingThroughPixel.value();
        // pixelDrawingAction(clickedPixelX, clickedPixelY...)
      }
      return;
    }

    case DrawingTool::Eraser:
      break;

    case DrawingTool::Rectangle:
      break;

    case DrawingTool::Circle:
      break;
  }

  // or maybe pass to base class
  event->ignore();
}

void DrawingWidget::wheelEvent(QWheelEvent* event) {
  if (event->modifiers() & Qt::ControlModifier) {
    const double angle = event->angleDelta().y();
    const double factor = qPow(1.0015, angle);

    const QPointF targetViewportPos = event->position();
    const QPointF targetScenePos = mapToScene(event->position().toPoint());

    scale(factor, factor);
    centerOn(targetScenePos);
    const QPointF deltaViewportPos = targetViewportPos - QPointF(
                                         viewport()->width() / 2.0,
                                         viewport()->height() / 2.0);
    const QPointF viewportCenter =
        mapFromScene(targetScenePos) - deltaViewportPos;
    centerOn(mapToScene(viewportCenter.toPoint()));
  } else {
    QGraphicsView::wheelEvent(event);
  }
}
}

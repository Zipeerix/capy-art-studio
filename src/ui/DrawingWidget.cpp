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
#include <QMouseEvent>
#include <QScrollBar>

namespace capy::ui {
DrawingWidget::DrawingWidget(QWidget* parent) :
  QGraphicsView(parent), _scene(new QGraphicsScene(this)),
  _drawing(0, 0), _settings(ConfigurationManager::createInstance()) {
  setScene(_scene);
  /*
  setViewportUpdateMode(FullViewportUpdate);
  setTransformationAnchor(AnchorUnderMouse);
  setResizeAnchor(AnchorViewCenter);*/
  //setRenderHint(QPainter::Antialiasing);
  //setTransformationAnchor(QGraphicsView::NoAnchor);
  //setResizeAnchor(QGraphicsView::NoAnchor);
}

void DrawingWidget::startNewDrawing(int width, int height) {
  _drawing = Drawing(width, height);
  redraw();
}

void DrawingWidget::redraw() {
  _scene->clear();

  const int width = _drawing.getWidth();
  const int height = _drawing.getHeight();

  _scene->setSceneRect(QRect(0, 0, width, height));
  centerOn(_scene->sceneRect().center());

  const auto pixmap = _drawing.convergeLayersIntoPixmap();
  if (pixmap.isNull()) {
    return;
  }

  _scene->addPixmap(pixmap);

  // TODO: Only show grid when zoomed in enough
  if (_settings->getDrawGrid()) {
    const int totalDrawingWidth = width;
    const int totalDrawingHeight = height;

    auto pen = QPen(Qt::lightGray);
    pen.setWidthF(0.1);
    pen.setCosmetic(true);

    for (int y = 0; y <= totalDrawingHeight; y += 1)
      _scene->addLine(0, y, totalDrawingHeight, y, pen);

    for (int x = 0; x <= totalDrawingWidth; x += 1)
      _scene->addLine(x, 0, x, totalDrawingWidth, pen);
  }
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

  switch (_tool) {
    case DrawingTool::Hand: {
      if (event->button() == Qt::LeftButton) {
        _rightMousePressed = true;
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
      }
    }

    case DrawingTool::Pen: {
      if (clickedPixel.has_value()) {
        _drawing.drawPixelOnCurrentLayer(clickedPixel->x(),
                                         clickedPixel->y(),
                                         QColor(0, 0, 0, 255));
        redraw();
      }
      return;
    }

    case DrawingTool::Eraser: // same as pen but set to invisible
      break;
    case DrawingTool::Rectangle:
      break;
    case DrawingTool::Circle:
      break;
  }

  // or maybe base to base class
  event->ignore();
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) {
  switch (_tool) {
    case DrawingTool::Hand: {
      if (event->button() == Qt::LeftButton) {
        _rightMousePressed = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
      }
    }
    case DrawingTool::Pen:
      break;
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

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) {
  const auto movingThroughPixel = mapPositionOfEventToScene(event);

  switch
  (_tool) {
    case DrawingTool::Hand: {
      if (_rightMousePressed) {
        horizontalScrollBar()->setValue(
            horizontalScrollBar()->value() - (event->x() - _panStartX));
        verticalScrollBar()->setValue(
            verticalScrollBar()->value() - (event->y() - _panStartY));
        _panStartX = event->x();
        _panStartY = event->y();
        event->accept();
        return;
      }
    }

    case DrawingTool::Pen: {
      if (movingThroughPixel.has_value()) {
        _drawing.drawPixelOnCurrentLayer(movingThroughPixel->x(),
                                         movingThroughPixel->y(),
                                         QColor(0, 0, 0, 255));
        redraw();
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

    const auto targetViewportPos = event->position();
    const auto targetScenePos = mapToScene(event->position().toPoint());

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

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
  _configurationManager(ConfigurationManager::createInstance()),
  _drawing(0, 0),
  _toolbox(this) {
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  recreateScene();
}

QByteArray DrawingWidget::createMiniatureBytes() const {
  return _drawingCanvasItem->createMiniatureBytes();
}

void DrawingWidget::drawBackground(QPainter* painter, [[maybe_unused]] const QRectF& rect) {
  painter->save();
  painter->resetTransform();
  // TODO: Below maybe change to rect?
  painter->drawTiledPixmap(viewport()->rect(), _checkerboardPixmap);
  painter->restore();
}

void DrawingWidget::setDrawing(Drawing drawing) {
  _drawing = std::move(drawing);
  redrawScreen();
}

void DrawingWidget::setDrawingColor(const QColor color) {
  _toolbox.getPenTool()->setColor(color);
}

QColor DrawingWidget::getDrawingColor() const {
  return _toolbox.getPenTool()->getColor();
}

const std::vector<Layer>& DrawingWidget::getLayers() const {
  return _drawing.getLayers();
}

void DrawingWidget::startNewDrawing(const int width, const int height) {
  logger::info(fmt::format(
      "Creating new image with dimensions {}x{} with per layer size of {} bytes",
      width, height, utils::calculateInMemorySizeOfImage(width, height)));
  _drawing = Drawing(width, height);
  redrawScreen();
}

void DrawingWidget::redrawScreen() {
  removeGrid();
  recreateScene();
  resetZoom();

  const auto drawingDimensions = _drawing.getDimensions();
  _drawingCanvasItem = new DrawingCanvasItem(drawingDimensions.getWidth(), drawingDimensions.getHeight());
  _scene->addItem(_drawingCanvasItem);

  syncInternalAndExternalDrawing();
}

void DrawingWidget::recreateScene() {
  if (_scene != nullptr) {
    _scene->clear();
    delete _scene;
  }

  _scene = new QGraphicsScene(this);
  setScene(_scene);
}

void DrawingWidget::redrawGrid() {
  removeGrid();

  if (_configurationManager->getGraphicsSetting<bool>(ConfigurationManager::GraphicsSetting::DrawGrid)) {
    auto pen = QPen(Qt::lightGray);
    pen.setWidthF(_configurationManager->getGraphicsSetting<double>(ConfigurationManager::GraphicsSetting::GridWidth));
    pen.setCosmetic(true);

    const auto drawingDimensions = _drawing.getDimensions();
    for (int y = 0; y <= drawingDimensions.getHeight(); y += 1) {
      _lines.push_back(_scene->addLine(0, y, drawingDimensions.getWidth(), y, pen));
    }

    for (int x = 0; x <= drawingDimensions.getWidth(); x += 1) {
      _lines.push_back(_scene->addLine(x, 0, x, drawingDimensions.getHeight(), pen));
    }
  }
}

void DrawingWidget::removeGrid() {
  for (const QGraphicsLineItem* line : _lines) {
    delete line;
  }

  _lines.clear();
}

void DrawingWidget::resetZoom() {
  resetTransform();
  _zoomFactor = 1.0;
}

void DrawingWidget::updateZoomLevel(const double factor){
  const auto oldZoomFactor = _zoomFactor;
  _zoomFactor *= factor;

  drawOrRemoveGridBasedOnZoomLevel(oldZoomFactor);

  scale(factor, factor);
}

void DrawingWidget::drawOrRemoveGridBasedOnZoomLevel(const double oldZoomFactor) {
  // TODO Prime example of checking configuration manager too ofter
  // TODO: Save threshold on startup and then have a connect so that if its changed somewhere else
  // TODO: the saved value is changed, so that config manager doesnt have to be checked every
  // TODO: zoom tick
  const auto zoomThreshold = _configurationManager->getGraphicsSetting<double>(ConfigurationManager::GraphicsSetting::GridDrawingZoomThreshold);
  if (oldZoomFactor <= zoomThreshold && _zoomFactor >= zoomThreshold) {
    redrawGrid();
  } else if (oldZoomFactor >= zoomThreshold && _zoomFactor <= zoomThreshold) {
    removeGrid();
  }
}

void DrawingWidget::syncInternalAndExternalDrawing() const {
  _drawingCanvasItem->updateAllPixels([&](const int x, const int y) {
    return _drawing.calculateCombinedPixelColor(x, y);
  });
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
  const auto drawingDimensions = _drawing.getDimensions();
  if (mappedPositionX >= 0 &&
      mappedPositionY >= 0 &&
      mappedPositionX < drawingDimensions.getWidth() &&
      mappedPositionY < drawingDimensions.getHeight()) {
    return QPoint{mappedPositionX, mappedPositionY};
  }

  return std::nullopt;
}

void DrawingWidget::drawPixelOnBothRepresentations(int x, int y, const QColor& drawingColor){
  _drawing.drawPixelOnCurrentLayerInternalRepresentationOnly(x, y, drawingColor);
  const auto combinedColor = _drawing.calculateCombinedPixelColor(x, y);
  _drawingCanvasItem->updateExternalCanvasPixel(x, y, combinedColor);
}

void DrawingWidget::mousePressEvent(QMouseEvent* event) {
  const auto clickedPixel = mapPositionOfEventToScene(event);
  if (_toolbox.getCurrentToolInterface()->mousePressEvent(event, clickedPixel)) {
    event->accept();
  } else {
    event->ignore();
  }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent* event) {
  const auto clickedPixel = mapPositionOfEventToScene(event);
  if (_toolbox.getCurrentToolInterface()->mouseReleaseEvent(event, clickedPixel)) {
    event->accept();
  } else {
    event->ignore();
  }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent* event) {
  const auto movingThroughPixel = mapPositionOfEventToScene(event);
  //const QPointF eventPosition = event->position();

  if (_toolbox.getCurrentToolInterface()->mouseMoveEvent(event, movingThroughPixel)) {
    event->accept();
  } else {
    event->ignore();
  }
}

void DrawingWidget::wheelEvent(QWheelEvent* event) {
  if (event->modifiers() & Qt::ControlModifier) {
    const double angle = event->angleDelta().y();
    const double factor = qPow(1.0015, angle);

    updateZoomLevel(factor);

    // TODO: Implement zoom on mouse, this doesnt work
    /*const QPointF targetViewportPos = event->position();
    const QPointF targetScenePos = mapToScene(event->position().toPoint());

    centerOn(targetScenePos);
    const QPointF deltaViewportPos = targetViewportPos - QPointF(
                                         viewport()->width() / 2.0,
                                         viewport()->height() / 2.0);
    const QPointF viewportCenter =
        mapFromScene(targetScenePos) - deltaViewportPos;
    centerOn(mapToScene(viewportCenter.toPoint())); */
  } else {
    QGraphicsView::wheelEvent(event);
  }
}
}

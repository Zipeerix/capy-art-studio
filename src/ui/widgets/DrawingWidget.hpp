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

#ifndef DRAWINGWIDGET_HPP
#define DRAWINGWIDGET_HPP

#include <QGraphicsView>
#include "graphics/Drawing.hpp"
#include "DrawingCanvasItem.hpp"
#include "graphics/DrawingTools.hpp"
#include "io/ConfigurationManager.hpp"
#include "utils/CheckerboardPixmap.hpp"

namespace capy::ui {
struct Pixel {
  uint8_t r, g, b, a;
};

class DrawingWidget final : public QGraphicsView {
public:
  explicit DrawingWidget(QWidget* parent);

  QByteArray createMiniatureBytes() const;

  void setDrawing(Drawing drawing);
  void startNewDrawing(int width, int height);
  void setCurrentLayer(int newLayer);
  void setDrawingColor(QColor color);

  QColor getDrawingColor() const;
  const std::vector<Layer>& getLayers() const;

  void drawBackground(QPainter* painter, const QRectF& rect) override;

  void resetZoom();

private:
  std::shared_ptr<ConfigurationManager> _configurationManager;
  CheckerboardPixmap _checkerboardPixmap;
  QColor _drawingColor = QColor(0, 0, 0, 255);

  std::vector<QGraphicsLineItem*> _lines;

  DrawingCanvasItem* _drawingCanvasItem = nullptr;
  QGraphicsScene* _scene = nullptr;

  Drawing _drawing;
  DrawingTool _tool = DrawingTool::Pen;

  std::optional<QPoint> _lastContinousDrawingPoint = std::nullopt;

  bool _leftMouseButtonPressed = false;
  int _panStartX = 0;
  int _panStartY = 0;

  double _zoomFactor = 1.0;

  std::optional<QPoint> mapPositionOfEventToScene(const QMouseEvent* event) const;

  void drawPixelOnBothRepresentations(int x, int y, const QColor& drawingColor);

  void redrawScreen();
  void redrawGrid();
  void removeGrid();
  void drawOrRemoveGridBasedOnZoomLevel(double oldZoomFactor);
  void recreateScene();

  void updateZoomLevel(double factor);

  void syncInternalAndExternalDrawing() const;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
};
}

#endif //DRAWINGWIDGET_HPP

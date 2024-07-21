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
#include <memory>
#include "graphics/drawing-tools/PenTool.hpp"
#include "graphics/Drawing.hpp"
#include "DrawingCanvasItem.hpp"
#include "graphics/drawing-tools/Toolbox.hpp"
#include "io/ConfigurationManager.hpp"
#include "ui/utils/CheckerboardPixmap.hpp"

namespace capy::ui {
struct Pixel {
  uint8_t r, g, b, a;
};

class DrawingWidget final : public QGraphicsView {
public:
  explicit DrawingWidget(QWidget* parent);

  QByteArray createMiniatureBytes() const;

  void drawPixelOnBothRepresentations(int x, int y, const QColor& drawingColor);

  void setDrawing(Drawing drawing);
  void startNewDrawing(int width, int height);
  void setCurrentLayer(int newLayer);
  void handleColorPickerColorChange(QColor color) const;

  void switchTool(DrawingTool drawingTool);

  const std::vector<Layer>& getLayers() const;

  void resetZoom();

private:
  std::shared_ptr<ConfigurationManager> _configurationManager;
  CheckerboardPixmap _checkerboardPixmap;

  std::vector<QGraphicsLineItem*> _lines;

  DrawingCanvasItem* _drawingCanvasItem = nullptr;
  QGraphicsScene* _scene = nullptr;

  Drawing _drawing;
  Toolbox _toolbox;

  double _zoomFactor = 1.0;

  void drawBackground(QPainter* painter, const QRectF& rect) override;

  std::optional<QPoint> mapPositionOfEventToScene(const QMouseEvent* event) const;

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

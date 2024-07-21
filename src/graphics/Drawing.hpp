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

#ifndef DRAWING_HPP
#define DRAWING_HPP

#include <vector>

#include "Layer.hpp"
#include "algorithms/AlphaBlending.hpp"
#include "utils/Dimensions.hpp"

namespace capy {
class Drawing {
 public:
  Drawing(int width, int height);

  void insertOrAssignLayerFromRawPixels(int index, const std::string& name,
                                        std::vector<Pixel> pixels);

  utils::Dimensions getDimensions() const;
  int getLayerCount() const;
  const Layer& getCurrentLayer() const;
  const std::vector<Layer>& getLayers() const;

  void setCurrentLayer(int newCurrentLayer);

  void drawPixelOnCurrentLayerInternalRepresentationOnly(int x, int y, const QColor& color);
  QColor calculateCombinedPixelColor(int x, int y) const;

 private:
  std::vector<Layer> _layers;
  utils::Dimensions _dimensions;
  int _currentLayer = 0;
};
}  // namespace capy

#endif  // DRAWING_HPP

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

#include "AlphaBlending.hpp"

#include <qmath.h>

#include <QDebug>
#include <utility>

#include "graphics/Colors.hpp"

namespace capy::algorithms {
AlphaBlender::AlphaBlender(PixelColorGettingFunction pixelColorGettingFunction)
    : _pixelColorGettingFunction(std::move(pixelColorGettingFunction)) {}

static ColorChannelValue getBlendChannelValue(const ColorChannelValue currentColor,
                                              const ColorChannelValue addedColor,
                                              const double alphaPercentage) {
  const double fromAdded = addedColor * alphaPercentage;
  const double fromCurrent = currentColor * (1.0 - alphaPercentage);

  return static_cast<ColorChannelValue>(std::round(fromAdded + fromCurrent));
}

static QColor applyAlphaBlendBetweenTwoPixels(const QColor& currentColor,
                                              const QColor& addedColor) {
  const double alphaPercentage = addedColor.alpha() / 255.0;
  const auto red = getBlendChannelValue(currentColor.red(), addedColor.red(), alphaPercentage);
  const auto green =
      getBlendChannelValue(currentColor.green(), addedColor.green(), alphaPercentage);
  const auto blue = getBlendChannelValue(currentColor.blue(), addedColor.blue(), alphaPercentage);
  const auto alpha =
      static_cast<uint8_t>(addedColor.alpha() + (currentColor.alpha() * (1.0 - alphaPercentage)));

  return {red, green, blue, alpha};
}

QColor AlphaBlender::blend(const int x, const int y, const int layerCount) const {
  // find first solid color to use as base
  std::optional<int> optIndexOfFirstSolidColor = std::nullopt;
  for (int currentLayerIndex = 0; currentLayerIndex < layerCount; currentLayerIndex++) {
    const auto& pixel = _pixelColorGettingFunction(x, y, currentLayerIndex);
    if (pixel.isSolid()) {
      optIndexOfFirstSolidColor = currentLayerIndex;
    }
  }

  // TODO: Maybe dont find first? since there is chance for double iteration
  const auto indexOfFirstSolidColor = optIndexOfFirstSolidColor.value_or(0);

  auto currentlyCalculatedPixelValue =
      _pixelColorGettingFunction(x, y, indexOfFirstSolidColor).convertToQColor();

  // apply blending to remaining layers
  for (int currentLayerIndex = indexOfFirstSolidColor; currentLayerIndex < layerCount;
       currentLayerIndex++) {
    const auto& pixel = _pixelColorGettingFunction(x, y, currentLayerIndex);

    // If there is no solid layer color then pretend the base layer is solid
    // even if its not
    if (pixel.isTransparent() && optIndexOfFirstSolidColor.has_value()) {
      continue;
    }

    auto currentPixelColor = pixel.convertToQColor();
    if (!optIndexOfFirstSolidColor.has_value() && currentLayerIndex == 0) {
      // Pretend base layer has solid alpha
      currentPixelColor.setAlpha(constants::alpha::solidColor);
    }

    currentlyCalculatedPixelValue =
        applyAlphaBlendBetweenTwoPixels(currentlyCalculatedPixelValue, currentPixelColor);
  }

  return currentlyCalculatedPixelValue;
}
}  // namespace capy::algorithms

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

#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <QBrush>
#include <cstdint>

#include "utils/Colors.hpp"

namespace capy {
class Pixel {
 public:
  Pixel(ColorChannelValue r, ColorChannelValue g, ColorChannelValue b,
        uint8_t alpha);

  static Pixel white(uint8_t alpha = constants::alpha::solidColor);
  static Pixel black(uint8_t alpha = constants::alpha::solidColor);

  void updateFromQColor(const QColor& color);

  [[nodiscard]] QColor convertToQColor() const;
  [[nodiscard]] QBrush convertToQBrush() const;

  [[nodiscard]] ColorChannelValue getRed() const;
  [[nodiscard]] ColorChannelValue getGreen() const;
  [[nodiscard]] ColorChannelValue getBlue() const;
  [[nodiscard]] ColorChannelValue getAlpha() const;

  [[nodiscard]] bool hasSomeTransparency() const;
  [[nodiscard]] bool isSolid() const;
  [[nodiscard]] bool isTransparent() const;

 private:
  ColorChannelValue _r;
  ColorChannelValue _g;
  ColorChannelValue _b;
  uint8_t _alpha;
};
}  // namespace capy

#endif  // PIXEL_HPP

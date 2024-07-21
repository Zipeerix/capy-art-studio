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

#include <cstdint>
#include <QBrush>

#include "Colors.hpp"

namespace capy
{
class Pixel
{
public:
  Pixel(ColorChannelValue r, ColorChannelValue g, ColorChannelValue b, uint8_t alpha);

  static Pixel white(uint8_t alpha = constants::alpha::solidColor);
  static Pixel black(uint8_t alpha = constants::alpha::solidColor);

  void updateFromQColor(const QColor& color);

  QColor convertToQColor() const;
  QBrush convertToQBrush() const;

  ColorChannelValue getRed() const;
  ColorChannelValue getGreen() const;
  ColorChannelValue getBlue() const;
  ColorChannelValue getAlpha() const;

  bool hasSomeTransparency() const;
  bool isSolid() const;
  bool isTransparent() const;

private:
  ColorChannelValue _r;
  ColorChannelValue _g;
  ColorChannelValue _b;
  uint8_t _alpha;
};
} // namespace capy

#endif // PIXEL_HPP

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

#include "Pixel.hpp"

namespace capy
{
Pixel::Pixel(const ColorChannelValue r, const ColorChannelValue g, const ColorChannelValue b,
             const uint8_t alpha) :
    _r(r),
    _g(g),
    _b(b),
    _alpha(alpha)
{
}

Pixel Pixel::white(const uint8_t alpha)
{
  return {constants::color::fullChannelValue, constants::color::fullChannelValue,
          constants::color::fullChannelValue, alpha};
}

Pixel Pixel::black(const uint8_t alpha)
{
  return {constants::color::minimumChannelValue, constants::color::minimumChannelValue,
          constants::color::minimumChannelValue, alpha};
}

void Pixel::updateFromQColor(const QColor& color)
{
  _r = color.red();
  _g = color.green();
  _b = color.blue();
  _alpha = color.alpha();
}

QColor Pixel::convertToQColor() const
{
  return {_r, _g, _b, _alpha};
}

QBrush Pixel::convertToQBrush() const
{
  return {convertToQColor()};
}

ColorChannelValue Pixel::getRed() const
{
  return _r;
}

ColorChannelValue Pixel::getBlue() const
{
  return _b;
}

ColorChannelValue Pixel::getGreen() const
{
  return _g;
}

ColorChannelValue Pixel::getAlpha() const
{
  return _alpha;
}

bool Pixel::hasSomeTransparency() const
{
  return _alpha < constants::alpha::solidColor;
}

bool Pixel::isSolid() const
{
  return _alpha == constants::alpha::solidColor;
}

bool Pixel::isTransparent() const
{
  return _alpha == constants::alpha::transparent;
}
} // namespace capy

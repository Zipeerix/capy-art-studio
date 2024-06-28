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

namespace capy {
using Color = uint8_t;

class Pixel {
 public:
  explicit Pixel(uint8_t alpha);
  Pixel(Color r, Color g, Color b, uint8_t alpha);

  void updateFromQColor(const QColor& color);

  [[nodiscard]] QColor convertToQColor() const;
  [[nodiscard]] QBrush convertToQBrush() const;

  [[nodiscard]] Color getRed() const;
  [[nodiscard]] Color getGreen() const;
  [[nodiscard]] Color getBlue() const;
  [[nodiscard]] Color getAlpha() const;

  [[nodiscard]] bool hasSomeTransparency() const;
  [[nodiscard]] bool isSolid() const;

 private:
  Color _r;
  Color _g;
  Color _b;
  uint8_t _alpha;
};
}  // namespace capy

#endif  // PIXEL_HPP

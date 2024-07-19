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

#ifndef DIMENSIONS_HPP
#define DIMENSIONS_HPP

namespace capy::utils {
class Dimensions {
 public:
  Dimensions(int width, int height);

  int getWidth() const;
  void setWidth(int newWidth);

  int getHeight() const;
  void setHeight(int newHeight);

  int getArea() const;

 private:
  int _width = 0;
  int _height = 0;
};
}  // namespace capy::utils

#endif  // DIMENSIONS_HPP

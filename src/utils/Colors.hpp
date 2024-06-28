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

#ifndef COLORS_HPP
#define COLORS_HPP

// TODO: move colorchannelvalue somewhere else and create constants.hpp for
// values
namespace capy {
using ColorChannelValue = uint8_t;

namespace constants::alpha {
constexpr ColorChannelValue solidColor = 255;
constexpr ColorChannelValue semiTransparent = 127;
constexpr ColorChannelValue transparent = 0;
}  // namespace constants::alpha
}  // namespace capy

#endif  // COLORS_HPP

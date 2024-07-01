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

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "algorithms/Luminance.hpp"

using namespace testing;

// TODO: value of luminance testing

TEST(algorithms, luminance_black_or_white_dark_color) {
  const auto result = capy::algorithms::blackOrWhiteBasedOnLuminance(QColor(5, 5, 5, 255));
  ASSERT_EQ(result, Qt::white);
}

TEST(algorithms, luminance_black_or_white_light_color) {
  const auto result = capy::algorithms::blackOrWhiteBasedOnLuminance(QColor(222, 253, 255, 255));
  ASSERT_EQ(result, Qt::black);
}

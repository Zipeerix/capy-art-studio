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

#include "algorithms/AlphaBlending.hpp"

using namespace testing;

TEST(algorithms, alpha_blending_one_layer)
{
  std::vector<capy::Pixel> layers = {
          capy::Pixel{128, 128, 128, 255},
  };

  const auto blender = capy::algorithms::AlphaBlender([&](int, int, int layer) -> capy::Pixel {
    return layers.at(layer);
  });

  const auto blendResult = blender.blend(0, 0, static_cast<int>(layers.size()));
  ASSERT_EQ(blendResult, QColor(128, 128, 128, 255));
}

TEST(algorithms, alpha_blending_two_layers)
{
  std::vector<capy::Pixel> layers = {capy::Pixel{128, 128, 128, 255}, capy::Pixel{0, 0, 0, 128}};

  const auto blender =
          capy::algorithms::AlphaBlender([&](const int, const int, const int layer) -> capy::Pixel {
            return layers.at(layer);
          });

  const auto blendResult = blender.blend(0, 0, static_cast<int>(layers.size()));
  ASSERT_EQ(blendResult, QColor(64, 64, 64, 255));
}

TEST(algorithms, alpha_blending_two_layers_transparent_base)
{
  std::vector<capy::Pixel> layers = {capy::Pixel{128, 128, 128, 0}, capy::Pixel{0, 0, 0, 128}};

  const auto blender =
          capy::algorithms::AlphaBlender([&](const int, const int, const int layer) -> capy::Pixel {
            return layers.at(layer);
          });

  const auto blendResult = blender.blend(0, 0, static_cast<int>(layers.size()));
  ASSERT_EQ(blendResult, QColor(64, 64, 64, 255));
}

TEST(algorithms, alpha_blending_n_layers)
{
  std::vector<capy::Pixel> layers = {
          capy::Pixel{128, 128, 128, 255}, capy::Pixel{0, 0, 0, 128},
          capy::Pixel{125, 66, 13, 128},   capy::Pixel{44, 44, 44, 75},
          capy::Pixel{191, 155, 21, 180},
  };

  const auto blender =
          capy::algorithms::AlphaBlender([&](const int, const int, const int layer) -> capy::Pixel {
            return layers.at(layer);
          });

  const auto blendResult = blender.blend(0, 0, static_cast<int>(layers.size()));
  ASSERT_EQ(blendResult, QColor(158, 127, 27, 254));
}

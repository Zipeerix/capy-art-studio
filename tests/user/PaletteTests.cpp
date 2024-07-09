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

#include "user/Palette.hpp"

using namespace testing;

// TODO: Negative tests

TEST(user, palette_json_loading) {
  const auto testJsonPath = "tests/user/resources/TestPalette.json";

  const auto paletteOpt = capy::Palette::createFromJson(testJsonPath);
  ASSERT_TRUE(paletteOpt.has_value());

  const auto& palette = paletteOpt.value();
  ASSERT_EQ(palette.getName(), "MyPalette");
  ASSERT_EQ(palette.colorCount(), 2);

  const auto firstColor = palette.getColor(0).color;
  ASSERT_EQ(firstColor.red(), 255);
  ASSERT_EQ(firstColor.green(), 128);
  ASSERT_EQ(firstColor.blue(), 64);
  ASSERT_EQ(firstColor.alpha(), 44);

  const auto secondColor = palette.getColor(1).color;
  ASSERT_EQ(secondColor.red(), 255);
  ASSERT_EQ(secondColor.green(), 0);
  ASSERT_EQ(secondColor.blue(), 0);
  ASSERT_EQ(secondColor.alpha(), 255);
}

TEST(user, palette_json_exporting) {
  const auto testJsonPathIn = "tests/user/resources/TestPalette.json";
  const auto testJsonPathOut = "tests/user/resources/TestPalette_export.json";

  const auto paletteOpt = capy::Palette::createFromJson(testJsonPathIn);
  ASSERT_TRUE(paletteOpt.has_value());

  auto palette = paletteOpt.value();
  palette.setName("NewName");

  const auto saveError = palette.saveToJson(testJsonPathOut);
  EXPECT_TRUE(!saveError.has_value());

  const auto savedPaletteOpt = capy::Palette::createFromJson(testJsonPathOut);
  ASSERT_TRUE(savedPaletteOpt.has_value());

  const auto& savedPalette = savedPaletteOpt.value();
  ASSERT_EQ(savedPalette.getName(), "NewName");
  ASSERT_EQ(savedPalette.colorCount(), 2);

  const auto firstColor = savedPalette.getColor(0).color;
  ASSERT_EQ(firstColor.red(), 255);
  ASSERT_EQ(firstColor.green(), 128);
  ASSERT_EQ(firstColor.blue(), 64);
  ASSERT_EQ(firstColor.alpha(), 44);

  const auto secondColor = savedPalette.getColor(1).color;
  ASSERT_EQ(secondColor.red(), 255);
  ASSERT_EQ(secondColor.green(), 0);
  ASSERT_EQ(secondColor.blue(), 0);
  ASSERT_EQ(secondColor.alpha(), 255);
}
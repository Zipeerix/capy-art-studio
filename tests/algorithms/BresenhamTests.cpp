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

#include "algorithms/Bresenham.hpp"

struct Point {
  int x;
  int y;

  bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

using namespace testing;

/** TODO Combine all tests into testBresenham function taht takes in coords and
 * vector of expected points */

TEST(algorithms, bresenham_same_point) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(0, 0, 0, 0,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{
      Point{0, 0},
  };

  ASSERT_EQ(points.size(), expectedPoints.size());
  ASSERT_EQ(points[0], expectedPoints[0]);
}

TEST(algorithms, bresenham_qpoint) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(QPoint(0, 0), QPoint(1, 1),
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{
      Point{0, 0},
      Point{1, 1},
  };

  ASSERT_EQ(points.size(), expectedPoints.size());
  ASSERT_EQ(points[0], expectedPoints[0]);
}

TEST(algorithms, bresenham_same_x) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(0, 0, 0, 3,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{
      Point{0, 0},
      Point{0, 1},
      Point{0, 2},
      Point{0, 3},
  };

  ASSERT_EQ(points.size(), expectedPoints.size());
  ASSERT_EQ(points[0], expectedPoints[0]);
}

TEST(algorithms, bresenham_same_y) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(0, 0, 3, 0,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{
      Point{0, 0},
      Point{1, 0},
      Point{2, 0},
      Point{3, 0},
  };

  ASSERT_EQ(points.size(), expectedPoints.size());
  ASSERT_EQ(points[0], expectedPoints[0]);
}

TEST(algorithms, bresenham_straight_line_upwards) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(0, 0, 5, 5,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints =
      std::vector{Point{0, 0}, Point{1, 1}, Point{2, 2}, Point{3, 3}, Point{4, 4}, Point{5, 5}};

  ASSERT_EQ(points.size(), expectedPoints.size());

  for (size_t i = 0; i < points.size(); i++) {
    auto actualPoint = points.at(i);
    auto expectedPoint = expectedPoints.at(i);

    ASSERT_EQ(actualPoint, expectedPoint);
  }
}

TEST(algorithms, bresenham_straight_line_downwards) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(5, 5, 0, 0,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{
      Point{5, 5}, Point{4, 4}, Point{3, 3}, Point{2, 2}, Point{1, 1}, Point{0, 0},
  };

  ASSERT_EQ(points.size(), expectedPoints.size());

  for (size_t i = 0; i < points.size(); i++) {
    auto actualPoint = points.at(i);
    auto expectedPoint = expectedPoints.at(i);

    ASSERT_EQ(actualPoint, expectedPoint);
  }
}

TEST(algorithms, bresenham_curved_line_upwards) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(0, 0, 2, 1,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{Point{0, 0}, Point{1, 1}, Point{2, 1}};

  ASSERT_EQ(points.size(), expectedPoints.size());

  for (size_t i = 0; i < points.size(); i++) {
    auto actualPoint = points.at(i);
    auto expectedPoint = expectedPoints.at(i);

    ASSERT_EQ(actualPoint, expectedPoint);
  }
}

TEST(algorithms, bresenham_curved_line_downwards) {
  std::vector<Point> points;

  capy::algorithms::applyBresenham(10, 10, 8, 9,
                                   [&](int x, int y) { points.push_back(Point{x, y}); });

  const auto expectedPoints = std::vector{Point{10, 10}, Point{9, 9}, Point{8, 9}};

  ASSERT_EQ(points.size(), expectedPoints.size());

  for (size_t i = 0; i < points.size(); i++) {
    auto actualPoint = points.at(i);
    auto expectedPoint = expectedPoints.at(i);

    ASSERT_EQ(actualPoint, expectedPoint);
  }
}

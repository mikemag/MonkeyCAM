/*
 * Copyright 2013 Michael M. Magruder (https://github.com/mikemag)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gtest/gtest.h>
#include "point.h"
#include "basic-math.h"

//------------------------------------------------------------------------------
// Unit tests for MonkeyCAM primitives

using MonkeyCAM::MCFixed;

TEST(MCFixedTest, Basic) {
  MCFixed x = 4.42;
  double d = x.dbl();
  EXPECT_EQ(d, 4.42) << "where x is " << x;
  EXPECT_EQ(44200, x.scaledInt());
}

TEST(MCFixedTest, Math) {
  MCFixed x = 4.42;
  MCFixed y = 2.0;
  EXPECT_EQ((x + y).scaledInt(), 64200);
  EXPECT_EQ((x - y).scaledInt(), 24200);
  EXPECT_EQ((x * y).scaledInt(), 88400);
  EXPECT_EQ((x / y).scaledInt(), 22100);
}

TEST(MCFixedTest, Comparisons) {
  MCFixed x = 4.42;
  MCFixed y = 2.0;
  EXPECT_TRUE(x == x);
  EXPECT_FALSE(x == y);
  EXPECT_TRUE(x != y);
  EXPECT_FALSE(x != x);
  EXPECT_TRUE(y < x);
  EXPECT_TRUE(y <= x);
  EXPECT_TRUE(x <= x);
  EXPECT_TRUE(x > y);
  EXPECT_TRUE(x >= y);
  EXPECT_TRUE(x >= x);
}

//------------------------------------------------------------------------------
// Basic math tests

TEST(BasicMath, Circle) {
  MonkeyCAM::Circle c(1, 1, 2, 2, 3, 1);
  EXPECT_EQ(c.centerX, 2.0);
  EXPECT_EQ(c.centerY, 1.0);
  EXPECT_EQ(c.radius, 1.0);
}

// @todo: need some path tests. Perhaps form paths of the different
// types and assert that some key points are correct: ends, a few
// mid-points, compute the bounding box and asser that it's within the
// correct limits.

int main (int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

using namespace testing;

TEST(placeholder, placeholder1) {
  EXPECT_EQ(2, 1);
  ASSERT_THAT(0, Eq(0));
}

#include "gtest/gtest.h"
#include "../inc/math/Math.hpp"

namespace g_math = geom::math;
TEST(MathTest, IsEqual) {
    EXPECT_TRUE(g_math::isEqual(1.0, 1.0)) << "Expected 1.0 and 1.0 to be equal";
    EXPECT_TRUE(g_math::isEqual(1.0, 1.0 + g_math::TOLERANCE / 2)) << "Expected 1.0 and 1.0 + TOLERANCE / 2 to be equal";
    EXPECT_FALSE(g_math::isEqual(1.0, 1.0 + g_math::TOLERANCE * 2)) << "Expected 1.0 and 1.0 + TOLERANCE * 2 to not be equal";
}

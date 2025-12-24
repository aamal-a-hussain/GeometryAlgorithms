#include "gtest/gtest.h"
#include "../inc/math/Line.hpp"

namespace g_math = geom::math;
TEST(LineTest, Constructor) {
    g_math::Vector2f origin(1.0, 2.0);
    g_math::Vector2f dir(3.0, 4.0);
    g_math::Line<float, 2> line(origin, dir);

    EXPECT_EQ(line.GetOrigin(), origin) << "Origin of line should be " << origin << " but got " << line.GetOrigin();
    EXPECT_EQ(line.GetDir(), dir) << "Direction of line should be " << dir << " but got " << line.GetDir();
}

TEST(LineTest, Getters) {
    g_math::Vector3f origin(1.0, 2.0, 3.0);
    g_math::Vector3f dir(4.0, 5.0, 6.0);
    g_math::Line<float, 3> line(origin, dir);

    EXPECT_EQ(line.GetOrigin(), origin) << "Origin of line should be " << origin << " but got " << line.GetOrigin();
    EXPECT_EQ(line.GetDir(), dir) << "Direction of line should be " << dir << " but got " << line.GetDir();
}

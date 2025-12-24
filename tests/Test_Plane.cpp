#include "../inc/math/Plane.hpp"
#include "../inc/math/Point.hpp"
#include <gtest/gtest.h>

namespace g_math = geom::math;
TEST(PlaneTest, CanCreatePlaneFrom3Points) {
    g_math::Point3f p1(1.0, 2.0, 3.0);
    g_math::Point3f p2(4.0, 5.0, 6.0);
    g_math::Point3f p3(7.0, 8.0, 10.0);

    g_math::Plane plane(p1, p2, p3);

    g_math::Vector3f v12 = p2 - p1;
    g_math::Vector3f v13 = p3 - p1;
    g_math::Vector3f exp_normal = g_math::Cross3D(v12, v13);
    float exp_d = g_math::Dot(exp_normal, p1);

    ASSERT_EQ(plane.GetNormal(), exp_normal);
    ASSERT_EQ(plane.GetD(), exp_d);
}

TEST(PlaneTest, CanCreatePlaneFromNormalAndD) {
    g_math::Vector3f normal(1.0, 2.0, 3.0);
    float d = 4.0;

    g_math::Plane plane(normal, d);

    ASSERT_EQ(plane.GetNormal(), normal);
    ASSERT_EQ(plane.GetD(), d);
}

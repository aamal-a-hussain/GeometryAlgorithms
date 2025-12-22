#include "gtest/gtest.h"
#include "../inc/math/GeomUtils.hpp"
namespace g_math = geom::math;
TEST(GeomUtilsTest, Area2D) {
    g_math::Vector2f a(1.0, 2.0);
    g_math::Vector2f b(3.0, 4.0);
    float got = g_math::area2d(a, b);
    EXPECT_FLOAT_EQ(got, -1.0) << "Area of " << a << " and " << b << " should be -1.0 but got " << got;
}

TEST(GeomUtilsTest, Area3D) {
    g_math::Vector3f a(1.0, 2.0, 3.0);
    g_math::Vector3f b(4.0, 5.0, 6.0);
    float got = g_math::area3d(a, b);
    EXPECT_FLOAT_EQ(got, 3.674234614174767) << "Area of " << a << " and " << b << " should be 0.0 but got " << got;
}

TEST(GeomUtilsTest, Coincident2D) {
    g_math::Vector2f a(1.0, 2.0);
    g_math::Vector2f b(2.0, 4.0);
    EXPECT_TRUE(g_math::coincident2d(a, b)) << a << " and " << b << " should be coincident but are not";
}

TEST(GeomUtilsTest, NotCoincident2D) {
    g_math::Vector2f a(1.0, 2.0);
    g_math::Vector2f b(2.0, 5.0);
    EXPECT_FALSE(g_math::coincident2d(a, b)) << a << " and " << b << " should not be coincident but are";
}

TEST(GeomUtilsTest, Coincident3D) {
    g_math::Vector3f a(1.0, 2.0, 3.0);
    g_math::Vector3f b(2.0, 4.0, 6.0);
    EXPECT_TRUE(g_math::coincident3d(a, b)) << a << " and " << b << " should be coincident but are not";
}

TEST(GeomUtilsTest, NotCoincident3D) {
    g_math::Vector3f a(1.0, 2.0, 3.0);
    g_math::Vector3f b(2.0, 4.0, 7.0);
    EXPECT_FALSE(g_math::coincident3d(a, b)) << a << " and " << b << " should not be coincident but are";
}

TEST(GeomUtilsTest, Orientation2D_Positive) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(1.0, 0.0);
    g_math::Point2f c(1.0, 1.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::POSITIVE) << "Orientation of " << a << ", " << b << ", " << c << " should be POSITIVE but got " << result;
}

TEST(GeomUtilsTest, Orientation2D_Negative) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(1.0, 0.0);
    g_math::Point2f c(1.0, -1.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::NEGATIVE) << "Orientation of " << a << ", " << b << ", " << c << " should be NEGATIVE but got " << result;
}

TEST(GeomUtilsTest, Orientation2D_Beyond) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(1.0, 0.0);
    g_math::Point2f c(2.0, 0.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::BEYOND) << "Orientation of " << a << ", " << b << ", " << c << " should be BEYOND but got " << result;
}

TEST(GeomUtilsTest, Orientation2D_Behind) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(1.0, 0.0);
    g_math::Point2f c(-1.0, 0.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::BEHIND) << "Orientation of " << a << ", " << b << ", " << c << " should be BEHIND but got " << result;
}

TEST(GeomUtilsTest, Orientation2D_InInterval) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(2.0, 0.0);
    g_math::Point2f c(1.0, 0.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::IN_INTERVAL) << "Orientation of " << a << ", " << b << ", " << c << " should be IN_INTERVAL but got " << result;
}

TEST(GeomUtilsTest, Orientation2D_Origin) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(1.0, 0.0);
    g_math::Point2f c(0.0, 0.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::ORIGIN) << "Orientation of " << a << ", " << b << ", " << c << " should be ORIGIN but got " << result;
}

TEST(GeomUtilsTest, Orientation2D_Destination) {
    g_math::Point2f a(0.0, 0.0);
    g_math::Point2f b(1.0, 0.0);
    g_math::Point2f c(1.0, 0.0);
    g_math::Orientation result = g_math::orientation2d(a, b, c);
    EXPECT_EQ(result, g_math::Orientation::DESTINATION) << "Orientation of " << a << ", " << b << ", " << c << " should be DESTINATION but got " << result;
}

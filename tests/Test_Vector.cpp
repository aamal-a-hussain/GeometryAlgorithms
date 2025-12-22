#include "../inc/math/Vector.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
namespace g_math = geom::math;
class VectorTestFixture : public::testing::Test {
    public:
        g_math::Vector<float, 3> v3 = g_math::Vector<float, 3>(
            std::array<float, 3> {1.0, 2.0, 3.0}
        );
        g_math::Vector2f v2f {3.0, 4.0};
        g_math::Vector3f v3f {3.0, 4.0, 5.0};
};

TEST(VectorTest, CanCreateZeroVector) {
    auto got = g_math::Vector<float, 3>();
    EXPECT_EQ(got.x(), 0.0) << "Expected x coordinate to be 0.0 but got " << got.x();
    EXPECT_EQ(got.y(), 0.0) << "Expected y coordinate to be 0.0 but got " << got.y();
    EXPECT_EQ(got.z(), 0.0) << "Expected z coordinate to be 0.0 but got " << got.z();
}

TEST(VectorTest, CanCreateVectorFromCoords) {
    float x=1.0, y=2.0, z=3.0;
    auto got = g_math::Vector<float, 3>(
        std::array<float, 3> {x, y, z}
    );
    EXPECT_EQ(got.x(), x) << "Expected x coordinate to be " << x << " but got " << got.x();
    EXPECT_EQ(got.y(), y) << "Expected y coordinate to be " << y << " but got " << got.y();
    EXPECT_EQ(got.z(), z) << "Expected z coordinate to be " << z << " but got " << got.z();
}

TEST(VectorTest, CanCreateVectorFromXYZ) {
    float x=1.0, y=2.0, z=3.0;
    auto got = g_math::Vector<float, 3>(x, y, z);
    EXPECT_EQ(got.x(), x) << "Expected x coordinate to be " << x << " but got " << got.x();
    EXPECT_EQ(got.y(), y) << "Expected y coordinate to be " << y << " but got " << got.y();
    EXPECT_EQ(got.z(), z) << "Expected z coordinate to be " << z << " but got " << got.z();
}

TEST(VectorTest, CanCreateVectorFromXY) {
    float x=1.0, y=2.0;
    auto got = g_math::Vector<float, 2>(x, y);
    EXPECT_EQ(got.x(), x) << "Expected x coordinate to be " << x << " but got " << got.x();
    EXPECT_EQ(got.y(), y) << "Expected y coordinate to be " << y << " but got " << got.y();
}

TEST(VectorTest, VectorsAreEqual) {
    float x=1.0, y=2.0;
    auto v1 = g_math::Vector<float, 2>(x, y);
    auto v2 = g_math::Vector<float, 2>(
        std::array<float, 2>{x, y}
    );
    ASSERT_EQ(v1, v2) << "Vectors " << v1 << " and " << v2 << " should be equal but are not";
}

TEST(VectorTest, VectorsAreNotEqual) {
    float x=1.0, y=2.0, z = 3.0, z2 = 4.0;
    auto v1 = g_math::Vector<float, 3>(x, y, z);
    auto v2 = g_math::Vector<float, 3>(
        std::array<float, 3>{x, y, z2}
    );
    ASSERT_TRUE(v1 != v2) << "Vectors " << v1 << " and " << v2 << " should not be equal but are";
}

TEST(VectorTest, CanSetVectorIdx) {
    float x=1.0, y=2.0, z = 3.0, z2 = 4.0;
    auto got = g_math::Vector<float, 3>(x, y, z);
    auto exp = g_math::Vector<float, 3>(
        std::array<float, 3>{x, y, z2}
    );
    got.set(2, z2);
    ASSERT_EQ(got, exp) << "Vectors " << exp << " and " << got << " should be equal but are not";
}

TEST(VectorTest, CanCreateUnitVectorInPlace) {
    float x=1.0, y=2.0, z = 3.0;
    auto u = g_math::Vector<float, 3>(x, y, z);
    u.to_unit_vector();
    float got = u.norm();
    ASSERT_NEAR(got, 1.0, 1e-6) << u << " should be a unit vector but has norm " << got;
}

TEST(VectorTest, CannotNormalizeZeroVector) {
    auto u = g_math::Vector3f(0.0, 0.0, 0.0);
    ASSERT_THROW(u.to_unit_vector(), std::runtime_error);
    ASSERT_THROW(std::ignore = u.normalize(), std::runtime_error);
}
TEST_F(VectorTestFixture, AddVectors) {
    auto exp = g_math::Vector3f {4.0, 6.0, 8.0};
    auto got = v3 + v3f;
    ASSERT_EQ(got, exp) << v3 << " + " << v3f << " = " << exp << " but got " << got;
}

TEST_F(VectorTestFixture, SubVectors) {
    auto exp = g_math::Vector3f {-2.0, -2.0, -2.0};
    auto got = v3 - v3f;
    ASSERT_EQ(got, exp) << v3 << " - " << v3f << " = " << exp << " but got " << got;
}

TEST_F(VectorTestFixture, NegateVector) {
    auto exp = g_math::Vector3f {-3.0, -4.0, -5.0};
    auto got = -v3f;
    ASSERT_EQ(got, exp) << "Negation of " << v3f << " should be " << exp << " but got " << got;
}

TEST_F(VectorTestFixture, CalculateVectorNorm) {
    float exp = 5.0;
    float got = v2f.norm();

    ASSERT_EQ(got, exp) << "Norm of " << v2f << " is 5.0 but got " << got;
}

TEST_F(VectorTestFixture, CanCreateUnitVector) {
    auto u = v3f.normalize();
    float got = u.norm();
    ASSERT_EQ(got, 1.0) << u << " should be a unit vector but has norm " << got;
}

TEST_F(VectorTestFixture, DotProductGivesNormSquared) {
    float got = g_math::dot(v3f, v3f);
    float norm = v3f.norm();
    float exp = norm * norm;
    ASSERT_NEAR(got, exp, 1e-6) << "Dot product of " << v3f << " with itself yields " << got << " but its norm squared is " << exp;
}

TEST_F(VectorTestFixture, DotProductOfOrthogonalVectorsIsZero) {
    auto v1 = g_math::Vector2f(1, 0);
    auto v2 = g_math::Vector2f(0, 1);
    float got = g_math::dot(v1, v2);
    float exp = 0;
    ASSERT_NEAR(got, exp, 1e-6) << "Dot product of " << v1 << " and " << v2 << " should be 0 but is " << got;
}

TEST_F(VectorTestFixture, DotProduct) {
    auto v1 = g_math::Vector2f(5, 3);
    auto v2 = g_math::Vector2f(2, 8);
    float got = g_math::dot(v1, v2);
    float exp = 34;
    ASSERT_NEAR(got, exp, 1e-6) << "Dot product of " << v1 << " and " << v2 << " should be 34 but is " << got;
}

TEST_F(VectorTestFixture, CrossProduct3D) {
    auto v1 = g_math::Vector3f(1, 0, 0);
    auto v2 = g_math::Vector3f(0, 1, 0);
    auto got = g_math::cross3D(v1, v2);
    auto exp = g_math::Vector3f(0, 0, 1);
    ASSERT_EQ(got, exp) << "Cross product of " << v1 << " and " << v2 << " should be " << exp << " but is " << got;
}

TEST_F(VectorTestFixture, CrossProduct2D) {
    auto v1 = g_math::Vector2f(1, 0);
    auto v2 = g_math::Vector2f(0, 1);
    float got = g_math::cross2D(v1, v2);
    float exp = 1;
    ASSERT_NEAR(got, exp, 1e-6) << "Cross product of " << v1 << " and " << v2 << " should be 1 but is " << got;
}

TEST_F(VectorTestFixture, IndexOutOfRange) {
    ASSERT_THROW(v3f[3], std::out_of_range);
}

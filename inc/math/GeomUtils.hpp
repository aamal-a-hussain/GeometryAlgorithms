#ifndef CPPGEOMETRY_GEOMUTILS_H
#define CPPGEOMETRY_GEOMUTILS_H

#include "Math.hpp"
#include "Vector.hpp"
#include "Point.hpp"

namespace geom
{

    namespace math
    {
        enum class Orientation
        {
            NEGATIVE,
            POSITIVE,
            BEYOND,
            BEHIND,
            IN_INTERVAL,
            ORIGIN,
            DESTINATION
        };

        inline std::ostream &operator<<(std::ostream &os, const Orientation &orientation)
        {
            switch (orientation)
            {
            case Orientation::NEGATIVE:
                os << "NEGATIVE";
                break;
            case Orientation::POSITIVE:
                os << "POSITIVE";
                break;
            case Orientation::BEYOND:
                os << "BEYOND";
                break;
            case Orientation::BEHIND:
                os << "BEHIND";
                break;
            case Orientation::IN_INTERVAL:
                os << "IN_INTERVAL";
                break;
            case Orientation::ORIGIN:
                os << "ORIGIN";
                break;
            case Orientation::DESTINATION:
                os << "DESTINATION";
                break;
            }
            return os;
        }

        float area2d(const Vector2f &a, const Vector2f &b)
        {
            return cross2D(a, b) / 2;
        }

        float area3d(const Vector3f &a, const Vector3f &b)
        {
            return cross3D(a, b).norm() / 2;
        }

        bool coincident2d(const Vector2f &a, const Vector2f &b)
        {
            auto area = area2d(a, b);
            return isEqual(area, 0.0);
        }

        bool coincident3d(const Vector3f &a, const Vector3f &b)
        {
            auto area = area3d(a, b);
            return isEqual(area, 0.0);
        }

        Orientation orientation2d(const Point2f &a, const Point2f &b, const Point2f &c)
        {
            if (a == c)
                return Orientation::ORIGIN;
            if (b == c)
                return Orientation::DESTINATION;

            Vector2f ab = b - a;
            Vector2f ac = c - a;

            auto area = area2d(ab, ac);
            if (isEqual(area, 0.0))
                area = 0.0;

            if (area < 0)
                return Orientation::NEGATIVE;
            if (area > 0)
                return Orientation::POSITIVE;

            // Vectors are coincident
            if (dot(ab, ac) < 0)
                return Orientation::BEHIND;
            if (ab.norm() < ac.norm())
                return Orientation::BEYOND;

            return Orientation::IN_INTERVAL;
        }

    } // namespace math
} // namespace geom

#endif // CPPGEOMETRY_GEOMUTILS_H

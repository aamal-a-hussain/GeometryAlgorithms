#ifndef CPPGEOMETRY_GEOMUTILS_H
#define CPPGEOMETRY_GEOMUTILS_H

#include "Math.hpp"
#include "Vector.hpp"
#include "Point.hpp"



namespace geom::math
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

        inline float Area2d(const Vector2f &a, const Vector2f &b)
        {
            return Cross2D(a, b) / 2;
        }

        inline float Area3d(const Vector3f &a, const Vector3f &b)
        {
            return Cross3D(a, b).Norm() / 2;
        }

        inline bool Coincident2d(const Vector2f &a, const Vector2f &b)
        {
            const auto area = Area2d(a, b);
            return IsEqual(area, 0.0f);
        }

        inline bool Coincident3d(const Vector3f &a, const Vector3f &b)
        {
            const auto area = Area3d(a, b);
            return IsEqual(area, 0.0f);
        }

        inline Orientation Orientation2d(const Point2f &a, const Point2f &b, const Point2f &c)
        {
            if (a == c)
                return Orientation::ORIGIN;
            if (b == c)
                return Orientation::DESTINATION;

            const Vector2f ab = b - a;
            const Vector2f ac = c - a;

            auto area = Area2d(ab, ac);
            if (IsEqual(area, 0.0f))
                area = 0.0;

            if (area < 0)
                return Orientation::NEGATIVE;
            if (area > 0)
                return Orientation::POSITIVE;

            // Vectors are coincident
            if (Dot(ab, ac) < 0)
                return Orientation::BEHIND;
            if (ab.Norm() < ac.Norm())
                return Orientation::BEYOND;

            return Orientation::IN_INTERVAL;
        }

        inline void LexicographicOrder(std::vector<math::Point2f>& points) {
            std::ranges::sort(points
                              , [](const Point2f &a, const Point2f &b) {
                                  return (a.x() < b.x()) || (IsEqual(a.x(), b.x()) && a.y() < b.y());
                              });
        }

    } // namespace geom::math


#endif // CPPGEOMETRY_GEOMUTILS_H

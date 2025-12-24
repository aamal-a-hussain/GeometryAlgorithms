//
// Created by aamalh on 12/24/25.
//

#ifndef CPPGEOMETRY_CONVEX_HULL_HPP
#define CPPGEOMETRY_CONVEX_HULL_HPP

#include <vector>

#include "../math/GeomUtils.hpp"
#include "../math/Point.hpp"

namespace geom {
namespace alg {
    inline std::vector<math::Point2f> HalfHull(const std::vector<math::Point2f>& points) {
        std::vector<math::Point2f> hull;
        hull.reserve(points.size());
        hull.push_back(points[0]);
        hull.push_back(points[1]);
        for (size_t i = 2; i < points.size(); i++) {
            hull.push_back(points[i]);
            while (hull.size() > 2) {
                const math::Orientation orientation = math::Orientation2d(
                    hull[hull.size() - 3],
                    hull[hull.size() - 2],
                    hull[hull.size() - 1]
                );
                if (orientation == math::Orientation::POSITIVE) break;
                // Otherwise remove middle of last three points
                hull.erase(hull.end()-2);
            }
        }
        return hull;
    }
    inline std::vector<math::Point2f> ConvexHull2D(const std::vector<math::Point2f>& points) {

        auto new_points = points;
        math::LexicographicOrder(new_points);
        std::vector<math::Point2f> upper_hull = HalfHull(new_points);

        std::ranges::reverse(new_points);
        std::vector<math::Point2f> lower_hull = HalfHull(new_points);

        upper_hull.insert(upper_hull.end(), lower_hull.begin() + 1, lower_hull.end() );
        return upper_hull;
    }
}
} // namespace geom

#endif // CPPGEOMETRY_CONVEX_HULL_HPP

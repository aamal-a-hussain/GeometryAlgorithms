//
// Created by Aamal Hussain on 05/12/2025.
//

#ifndef CPPGEOMETRY_MATH_H
#define CPPGEOMETRY_MATH_H

#include <cmath>

namespace geom {
namespace math {
    static constexpr float TOLERANCE = 1e-6;
    inline bool IsEqual(const double _x, const double _y){
        return fabs(_x - _y) < TOLERANCE;
    }
} // namespace math
} // namespace geom

#endif //CPPGEOMETRY_MATH_H

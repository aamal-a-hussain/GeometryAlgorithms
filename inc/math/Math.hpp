//
// Created by Aamal Hussain on 05/12/2025.
//

#ifndef CPPGEOMETRY_MATH_H
#define CPPGEOMETRY_MATH_H

#include <cmath>


namespace geom::math {
    template <std::floating_point T>
    bool IsEqual(const T _x, const T _y){
        return std::abs(_x - _y) < std::numeric_limits<T>::epsilon() * 100;
    }
    inline bool _xor(const bool _a, const bool _b) {
        return _a ^ _b;
    }
} // namespace geom::math


#endif //CPPGEOMETRY_MATH_H

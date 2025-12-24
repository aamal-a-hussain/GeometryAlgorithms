#ifndef CPPGEOMETRY_LINE_H
#define CPPGEOMETRY_LINE_H

#include "Vector.hpp"
#include <stdexcept>
namespace geom {
namespace math {
template <class T, size_t dim = DIM3> class Line {
  Vector<T, dim> origin;
  Vector<T, dim> dir;

public:
  Line() = delete;
  Line(const Vector<T, dim> &origin, const Vector<T, dim> &dir)
      : origin(origin), dir(dir){
        if (this->dir == 0.0) {
            throw std::runtime_error("Require non-zero direction for a line");
        }
      };

  const inline Vector<T, dim>& GetOrigin() { return origin; }
  const inline Vector<T, dim>& GetDir() { return dir; }
};
} // namespace math
} // namespace geom
#endif // CPPGEOMETRY_LINE_H

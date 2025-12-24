#ifndef CPPGEOMETRY_PLANE_H
#define CPPGEOMETRY_PLANE_H

#include "Point.hpp"
#include "Vector.hpp"
#include <stdexcept>
namespace geom {
    namespace math {
    class Plane {
        Vector3f _normal;
        float _d;

        public:
            Plane() = delete;
            Plane(const Vector3f& normal, const float d) :  _d(d) {
                if (normal == 0.0) {
                    throw std::runtime_error("Cannot create plane from zero normal vector");
                }
                _normal = normal;
            };
            Plane(const Point3f& p1, const Point3f& p2, const Point3f& p3) {
                if (p1 == p2 || p1 == p3 || p2 == p3) {
                    throw std::runtime_error("Require three unique points to define a plane");
                }
                const Vector3f v12 = p2 - p1;
                const Vector3f v13 = p3 - p1;

                _normal = Cross3D(v12, v13);
                _d = Dot(_normal, p1);
            }

            const Vector3f& GetNormal() const {
                return _normal;
            }

            const float& GetD() const {
                return _d;
            }
    };
} // namespace math
} // namespace geom
#endif //CPPGEOMETRY_PLANE_H

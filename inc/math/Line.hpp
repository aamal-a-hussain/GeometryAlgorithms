#ifndef CPPGEOMETRY_LINE_H
#define CPPGEOMETRY_LINE_H

#include "Vector.hpp"
#include <stdexcept>

#include "GeomUtils.hpp"


namespace geom::math {
        template<std::floating_point T, size_t dim = DIM3>
        class Line {
            Vector<T, dim> origin;
            Vector<T, dim> dest;

        public:
            Line() = delete;

            Line(const Vector<T, dim> &origin, const Vector<T, dim> &dest)
                : origin(origin), dest(dest) {
                if (this->dest == this->origin) {
                    throw std::runtime_error("Require two unique points for a line");
                }
            };

            [[nodiscard]] const Vector<T, dim> &GetOrigin() const { return origin; }
            [[nodiscard]] const Vector<T, dim> &GetDest() const { return dest; }
            [[nodiscard]] const Vector<T, dim> GetDir() const { return dest - origin; }

            bool Contains(const Vector<T, dim> &point) const {
                auto orientation = Orientation2d(origin, dest, point);
                return orientation == Orientation::ORIGIN
                       ||  orientation == Orientation::DESTINATION
                       ||  orientation == Orientation::IN_INTERVAL;
            }

            bool Intersects(const Line &other) const requires (dim == DIM2) {
                if (this->Contains(other.origin)
                    || this->Contains(other.dest)
                    || other.Contains(this->origin)
                    || other.Contains(this->dest)
                    ) return true;

                auto other_origin = Orientation2d(origin, dest, other.origin);
                auto other_dest = Orientation2d(origin, dest, other.dest);
                auto this_origin = Orientation2d(other.origin, other.dest, origin);
                auto this_dest = Orientation2d(other.origin, other.dest, dest);

                return _xor(other_origin == Orientation::NEGATIVE, other_dest == Orientation::NEGATIVE)
                       && _xor(this_origin == Orientation::NEGATIVE, this_dest == Orientation::NEGATIVE);
            }

        };

        inline Point2f GetIntersection(const Line<float, 2>& a, const Line<float, 2> &b)  {
        const auto dir_b = b.GetDir();
        const auto normal_b = Vector2f {-dir_b.y(), dir_b.x()};

        const auto t = Dot(normal_b, a.GetOrigin() - b.GetOrigin()) / Dot(normal_b, a.GetDest() - a.GetOrigin());
        return a.GetOrigin() + a.GetDir() * std::abs(t);
    }
    } // namespace geom::math

#endif // CPPGEOMETRY_LINE_H

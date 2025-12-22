//
// Created by Aamal Hussain on 05/12/2025.
//

#ifndef CPPGEOMETRY_VECTOR_H
#define CPPGEOMETRY_VECTOR_H
#include "Math.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <numeric>
#include <ostream>
#include <ranges>
#include <stdexcept>
#include <type_traits>

namespace geom {
namespace math {
static constexpr size_t DIM2 = 2;
static constexpr size_t DIM3 = 3;

template <class T, size_t dim> class Vector {
  static_assert(std::is_arithmetic_v<T>, "Vector class must be arithmetic");
  static_assert(dim >= DIM2,
                "Vector dimension must be at least two dimensional");

  std::array<T, dim> _coords;

public:
  Vector() = default;

  Vector(std::array<T, dim> coords) : _coords(coords){};

  template <typename = std::enable_if<dim == 3>>
  Vector(T x, T y, T z) : _coords({x, y, z}){};

  template <typename = std::enable_if<dim == 2>>
  Vector(T x, T y) : _coords({x, y}){};

  bool operator==(const Vector &other) const {
    return std::equal(
        _coords.begin(), _coords.end(), other._coords.begin(),
        [](const auto &a, const auto &b) { return isEqual(a, b); });
  }

  bool operator==(const T other) const {
    return std::all_of(
        _coords.begin(), _coords.end(),
        [other](const auto &a) { return isEqual(a, other); });
  }

  bool operator!=(const Vector &other) const {
    return !Vector::operator==(other);
  }

  bool operator!=(const T other) const {
    return !Vector::operator==(other);
  }

  Vector operator+(const Vector &other) const {
    Vector result;
    std::transform(_coords.begin(), _coords.end(), other._coords.begin(),
                   result._coords.begin(),
                   [](const T &a, const T &b) { return a + b; });
    return result;
  }

  Vector operator+(const T other) const {
    Vector result;
    std::transform(_coords.begin(), _coords.end(),
                   result._coords.begin(),
                   [other](const T &a) { return a + other; });
    return result;
  }

  Vector operator-(const Vector &other) const {
    Vector result;
    std::transform(_coords.begin(), _coords.end(), other._coords.begin(),
                   result._coords.begin(),
                   [](const T &a, const T &b) { return a - b; });
    return result;
  }

  Vector operator-(const T other) const {
    Vector result;
    std::transform(_coords.begin(), _coords.end(),
                   result._coords.begin(),
                   [other](const T& a) { return a - other; });
    return result;
  }

  Vector operator-() const {
    Vector result;
    std::transform(_coords.begin(), _coords.end(), result._coords.begin(),
                   [](const T &a) { return -a; });
    return result;
  }

  T operator[](size_t idx) const {
    if (idx >= dim)
      throw std::out_of_range("Vector index out of range");
    return _coords[idx];
  }

  void set(size_t idx, T value) {
    if (idx >= dim)
      throw std::out_of_range("Vector index out of range");
    _coords[idx] = value;
  }

  [[nodiscard]] inline T x() const
    requires(dim <= DIM3)
  {
    return _coords[0];
  }

  [[nodiscard]] inline T y() const
    requires(dim <= DIM3)
  {
    return _coords[1];
  }

  [[nodiscard]] inline T z() const
    requires(dim == DIM3)
  {
    return _coords[2];
  }

  [[nodiscard]] float norm() const {
    auto squares_view =
        _coords | std::views::transform([](T n) { return n * n; });
    float result =
        std::accumulate(squares_view.begin(), squares_view.end(), 0.0);
    return sqrt(result);
  }
  void to_unit_vector() {
    float magnitude = norm();
    if (isEqual(magnitude, 0.0)) throw std::runtime_error("Tried to normalize a vector with zero norm");
    std::for_each(_coords.begin(), _coords.end(),
                  [magnitude](T &c) { c /= magnitude; });
  }
  [[nodiscard]] Vector normalize() const {
    std::array<T, dim> result;
    float magnitude = norm();
    if (isEqual(magnitude, 0.0)) throw std::runtime_error("Tried to normalize a vector with zero norm");
    std::transform(_coords.begin(), _coords.end(), result.begin(),
                   [magnitude](const T &c) { return c / magnitude; });
    return {result};
  }
};

template <class T, size_t dim = DIM3>
float dot(const Vector<T, dim> &a, const Vector<T, dim> &b) {
  float result = 0;
  for (size_t i = 0; i < dim; i++) {
    result += a[i] * b[i];
  }

  return result;
}

template <class T, size_t dim = DIM3>
std::ostream &operator<<(std::ostream &os, const Vector<T, dim> &v) {
  os << "{";
  for (int i = 0; i < dim; ++i) {
    if (i != 0)
      os << ", ";
    os << v[i];
  }
  os << "}";
  return os;
}

typedef Vector<float, DIM2> Vector2f;
typedef Vector<float, DIM3> Vector3f;

inline float cross2D(const Vector2f &a, const Vector2f &b) {
  return a.x() * b.y() - a.y() * b.x();
}

inline Vector3f cross3D(const Vector3f &a, const Vector3f &b) {
  float x = a.y() * b.z() - a.z() * b.y();
  float y = a.z() * b.x() - a.x() * b.z();
  float z = a.x() * b.y() - a.y() * b.x();

  return Vector<float, 3>(x, y, z);
}

} // namespace math
} // namespace geom
#endif // CPPGEOMETRY_VECTOR_H

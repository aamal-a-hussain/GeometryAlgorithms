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
#include <concepts>


namespace geom::math {
static constexpr size_t DIM2 = 2;
static constexpr size_t DIM3 = 3;

template <std::floating_point T, size_t dim> requires (dim >= 2)
  class Vector {

  std::array<T, dim> _coords;

public:
  Vector() : _coords{} {};

  explicit Vector(std::array<T, dim> coords) : _coords(coords){};

  Vector(T x, T y, T z) requires (dim == 3) : _coords({x, y, z}){};

  Vector(T x, T y) requires (dim == 2) : _coords({x, y}){};

  bool operator==(const Vector &other) const {
    return std::equal(
        _coords.begin(), _coords.end(), other._coords.begin(),
        [](const auto &a, const auto &b) { return IsEqual<T>(a, b); });
  }

  bool operator==(const T other) const {
    return std::all_of(
        _coords.begin(), _coords.end(),
        [other](const auto &a) { return IsEqual<T>(a, other); });
  }

  bool operator!=(const Vector &other) const {
    return !Vector::operator==(other);
  }

  bool operator!=(const T other) const {
    return !Vector::operator==(other);
  }

  Vector operator+(const Vector &other) const {
    Vector result {};
    std::transform(_coords.begin(), _coords.end(), other._coords.begin(),
                   result._coords.begin(),
                   [](const T &a, const T &b) { return IsEqual<T>(a, 0) ? b : IsEqual<T>(b, 0) ? a : a + b; });
    return result;
  }

  Vector operator+(const T other) const {
    Vector result {};
    std::transform(_coords.begin(), _coords.end(),
                   result._coords.begin(),
                   [other](const T &a) { return IsEqual<T>(a, 0) ? other : a + other; });
    return result;
  }

  Vector operator-(const Vector &other) const {
    Vector result {};
    std::transform(_coords.begin(), _coords.end(), other._coords.begin(),
                   result._coords.begin(),
                   [](const T &a, const T &b) { return IsEqual<T>(a, 0) ? -b : IsEqual<T>(b, 0) ? a : a - b; });
    return result;
  }

  Vector operator-(const T other) const {
    Vector result {};
    std::transform(_coords.begin(), _coords.end(),
                   result._coords.begin(),
                   [other](const T& a) { return a - other; });
    return result;
  }

  Vector operator-() const {
    Vector result {};
    std::transform(_coords.begin(), _coords.end(), result._coords.begin(),
                   [](const T &a) { return IsEqual<T>(a, 0) ? 0 : -a; });
    return result;
  }

  Vector operator*(T other) const {
    Vector result {};
    std::transform(
      _coords.begin(), _coords.end(),
      result._coords.begin(),
      [other](const T &a) { return IsEqual<T>(a, 0) ? 0 : a * other; }
      );
    return result;
  }

  T operator[](size_t idx) const {
    if (idx >= dim)
      throw std::out_of_range("Vector index out of range");
    return _coords[idx];
  }


  [[nodiscard]] T x() const
    requires(dim <= DIM3)
  {
    return _coords[0];
  }

  [[nodiscard]] T y() const
    requires(dim <= DIM3)
  {
    return _coords[1];
  }

  [[nodiscard]] T z() const
    requires(dim == DIM3)
  {
    return _coords[2];
  }

  [[nodiscard]] float Norm() const {
    auto squares_view =
        _coords | std::views::transform([](T n) { return n * n; });
    const float result =
        std::accumulate(squares_view.begin(), squares_view.end(), 0.0);
    return sqrt(result);
  }
  void ToUnitVector() {
    float magnitude = Norm();
    if (IsEqual<T>(magnitude, 0.0)) throw std::runtime_error("Tried to normalize a vector with zero norm");
    std::for_each(_coords.begin(), _coords.end(),
                  [magnitude](T &c) { c /= magnitude; });
  }
  [[nodiscard]] Vector Normalise() const {
    std::array<T, dim> result {};
    float magnitude = Norm();
    if (IsEqual<T>(magnitude, 0.0)) throw std::runtime_error("Tried to normalize a vector with zero norm");
    std::transform(_coords.begin(), _coords.end(), result.begin(),
                   [magnitude](const T &c) { return c / magnitude; });
    return Vector(result);
  }

  };

template <class T, size_t dim = DIM3>
float Dot(const Vector<T, dim> &a, const Vector<T, dim> &b) {
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

inline float Cross2D(const Vector2f &a, const Vector2f &b) {
  return a.x() * b.y() - a.y() * b.x();
}

inline Vector3f Cross3D(const Vector3f &a, const Vector3f &b) {
  float x = a.y() * b.z() - a.z() * b.y();
  float y = a.z() * b.x() - a.x() * b.z();
  float z = a.x() * b.y() - a.y() * b.x();

  return {x, y, z};
}

} // namespace geom::math

#endif // CPPGEOMETRY_VECTOR_H

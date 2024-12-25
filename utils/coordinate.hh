#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <array>
#include <cmath>
#include <compare>  // IWYU pragma: keep
#include <type_traits>

namespace Utils {

template <typename INTEGER_TYPE>
  requires std::is_integral_v<INTEGER_TYPE>
struct CoordinateBase;

}  // namespace Utils

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator/(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x / rhs.x, .y = lhs.y / rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator/(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    INTEGER_TYPE rhs) -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x / rhs, .y = lhs.y / rhs};
}

namespace Utils {

template <typename INTEGER_TYPE>
  requires std::is_integral_v<INTEGER_TYPE>
struct CoordinateBase {
  INTEGER_TYPE x{};
  INTEGER_TYPE y{};

  [[nodiscard]] constexpr auto operator<=>(const CoordinateBase&) const =
      default;

  // Math

  constexpr auto operator+=(const CoordinateBase& other) -> CoordinateBase& {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr auto operator-=(const CoordinateBase& other) -> CoordinateBase& {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  // Transform

  constexpr void rotateClockwise() { *this = CoordinateBase{-y, x}; }

  constexpr void rotateCounterClockwise() { *this = CoordinateBase{y, -x}; }

  constexpr void flip() { *this = CoordinateBase{-x, -y}; }

  // Info

  [[nodiscard]] constexpr auto isZero() const -> bool {
    return x == 0 and y == 0;
  }

  [[nodiscard]] constexpr auto normalized() const -> CoordinateBase {
    if (x == 0 and y == 0) return {};
    return *this / std::max(std::abs(x), std::abs(y));
  }

  [[nodiscard]] constexpr auto neighbors() const
      -> std::array<CoordinateBase, 8> {
    return {CoordinateBase{x - 1, y - 1},
            {x, y - 1},
            {x + 1, y - 1},
            {x - 1, y},
            {x + 1, y},
            {x - 1, y + 1},
            {x, y + 1},
            {x + 1, y + 1}};
  }

  [[nodiscard]] constexpr auto neighborsUpDownLeftRight() const
      -> std::array<CoordinateBase, 4> {
    return {
        CoordinateBase{x, y - 1},  // Up
        {x, y + 1},                // Down
        {x - 1, y},                // Left
        {x + 1, y}                 // Right
    };
  }

  [[nodiscard]] constexpr auto neighorsDiagonal() const
      -> std::array<CoordinateBase, 4> {
    return {CoordinateBase{x - 1, y - 1},
            {x + 1, y + 1},
            {x + 1, y - 1},
            {x - 1, y + 1}};
  }

  [[nodiscard]] constexpr auto distanceFrom(const CoordinateBase& other) const
      -> double {
    return std::sqrt((x - other.x) * (x - other.x) +
                     (y - other.y) * (y - other.y));
  }

  [[nodiscard]] constexpr auto manhattanDistanceFrom(
      const CoordinateBase& other) const -> INTEGER_TYPE {
    return std::abs(x - other.x) + std::abs(y - other.y);
  }
};

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto rotatedClockwise(
    CoordinateBase<INTEGER_TYPE> coordinate) -> CoordinateBase<INTEGER_TYPE> {
  coordinate.rotateClockwise();
  return coordinate;
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto rotatedCounterClockwise(
    CoordinateBase<INTEGER_TYPE> coordinate) -> CoordinateBase<INTEGER_TYPE> {
  coordinate.rotateCounterClockwise();
  return coordinate;
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto flipped(CoordinateBase<INTEGER_TYPE> coordinate)
    -> CoordinateBase<INTEGER_TYPE> {
  coordinate.flip();
  return coordinate;
}

using Coordinate = CoordinateBase<int>;

}  // namespace Utils

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator+(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator-(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator*(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x * rhs.x, .y = lhs.y * rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator+(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    INTEGER_TYPE rhs) -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x + rhs, .y = lhs.y + rhs};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator-(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    INTEGER_TYPE rhs) -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x - rhs, .y = lhs.y - rhs};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator*(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    INTEGER_TYPE rhs) -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x * rhs, .y = lhs.y * rhs};
}

#endif  // COORDINATE_HH

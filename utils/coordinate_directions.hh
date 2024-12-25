#ifndef COORDINATE_DIRECTIONS_HH
#define COORDINATE_DIRECTIONS_HH

#include "coordinate.hh"

namespace Utils {

struct Direction {
  [[nodiscard]] static constexpr auto left() -> Coordinate {
    return {.x = -1, .y = 0};
  }

  [[nodiscard]] static constexpr auto right() -> Coordinate {
    return {.x = 1, .y = 0};
  }

  [[nodiscard]] static constexpr auto up() -> Coordinate {
    return {.x = 0, .y = -1};
  }

  [[nodiscard]] static constexpr auto down() -> Coordinate {
    return {.x = 0, .y = 1};
  }

  [[nodiscard]] static constexpr auto upLeft() -> Coordinate {
    return up() + left();
  }

  [[nodiscard]] static constexpr auto upRight() -> Coordinate {
    return up() + right();
  }

  [[nodiscard]] static constexpr auto downLeft() -> Coordinate {
    return down() + left();
  }

  [[nodiscard]] static constexpr auto downRight() -> Coordinate {
    return down() + right();
  }
};

struct Directions {
  [[nodiscard]] static constexpr auto clockwise() -> std::array<Coordinate, 8> {
    return {Direction::up(),        Direction::upRight(), Direction::right(),
            Direction::downRight(), Direction::down(),    Direction::downLeft(),
            Direction::left(),      Direction::upLeft()};
  }

  [[nodiscard]] static constexpr auto orthogonal()
      -> std::array<Coordinate, 4> {
    return {Direction::up(), Direction::right(), Direction::down(),
            Direction::left()};
  }

  [[nodiscard]] static constexpr auto diagonal() -> std::array<Coordinate, 4> {
    return {Direction::upRight(), Direction::downRight(), Direction::downLeft(),
            Direction::upLeft()};
  }
};

}  // namespace Utils

#endif  // COORDINATE_DIRECTIONS_HH

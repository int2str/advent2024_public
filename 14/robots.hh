#ifndef DAY_14_ROBOTS_HH
#define DAY_14_ROBOTS_HH

#include <filesystem>
#include <vector>

#include "utils/coordinate.hh"

namespace Day14 {

struct Robot {
  Utils::Coordinate position{};
  Utils::Coordinate velocity{};

  constexpr auto step() -> Robot& {
    position += velocity;
    return *this;
  }

  constexpr auto steps(int steps) -> Robot& {
    position += velocity * steps;
    return *this;
  }

  constexpr auto clipTo(Utils::Coordinate max) -> Robot& {
    if (position.y < 0) position.y = max.y + (position.y % max.y);
    if (position.y >= max.y) position.y = position.y % max.y;
    if (position.x < 0) position.x = max.x + (position.x % max.x);
    if (position.x >= max.x) position.x = position.x % max.x;
    return *this;
  }
};

using Robots = std::vector<Robot>;

[[nodiscard]] auto robotsFromFile(const std::filesystem::path& path) -> Robots;

[[nodiscard]] auto distanceBetween(const Robots& robots) -> double;

[[nodiscard]] auto distanceFrom(const Robots& robots,
                                Utils::Coordinate where) -> double;

}  // namespace Day14

#endif  // DAY_14_ROBOTS_HH

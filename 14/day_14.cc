//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/14
//

#include <algorithm>  // IWYU pragma: keep
#include <ranges>

#include "robots.hh"
#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"

namespace Day14 {

[[nodiscard]] constexpr auto safetyFactor(const Robots& robots,
                                          Utils::Coordinate max) -> size_t {
  const auto observe_robot = [&](Robot robot) {
    return robot.steps(100).clipTo(max);
  };
  const auto ignore_center = [&](Robot robot) {
    const auto center = max / 2;
    return robot.position.x != center.x and robot.position.y != center.y;
  };
  const auto to_quadrant = [&](Robot robot) {
    const auto center = max / 2;
    return (robot.position.y > center.y ? 2U : 0) |
           (robot.position.x > center.x ? 1U : 0);
  };
  const auto count = [](auto acc, auto quadrant) {
    ++acc[quadrant];
    return acc;
  };

  auto quadrants = robots                                  //
                   | std::views::transform(observe_robot)  //
                   | std::views::filter(ignore_center)     //
                   | std::views::transform(to_quadrant);

  return std::ranges::fold_left(
      std::ranges::fold_left(quadrants, std::array<size_t, 4>{}, count), 1U,
      std::multiplies{});
}

[[nodiscard]] auto detectAnomaly(Robots robots, Utils::Coordinate max,
                                 double threshold) {
  auto steps = size_t{};
  while (distanceFrom(robots, max / 2) > threshold) {
    for (auto& robot : robots) robot.step().clipTo(max);
    ++steps;
  }
  return steps;
}

}  // namespace Day14

TEST(Day_14_Restroom_Redoubt_SAMPLE) {
  const auto robots    = Day14::robotsFromFile("14/sample.txt");
  const auto grid_size = Utils::Coordinate{11, 7};
  EXPECT_EQ(Day14::safetyFactor(robots, grid_size), 12);
  // No sample solution provided for part 2
}

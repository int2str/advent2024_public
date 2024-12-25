//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/14
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <vector>

#include "robots.hh"
#include "utils/coordinate.hh"
#include "window.hh"

namespace Day14 {

void animate(std::vector<Robot> robots, Utils::Coordinate max) {
  auto steps = size_t{};

  auto window     = Window(max);
  auto min        = std::numeric_limits<double>::max();
  auto anomaly    = robots;
  auto anomaly_at = steps;

  while (window.isOpen()) {
    window.handleEvents();

    ++steps;
    for (auto& robot : robots) robot.step().clipTo(max);

    const auto average_distance = distanceBetween(robots);
    if (average_distance <= min) {
      min        = average_distance;
      anomaly    = robots;
      anomaly_at = steps;
      fmt::print("{:6.1F} @ {}\n", min, steps);
    }
    window.draw(anomaly, anomaly_at, min, steps);
  }
}

}  // namespace Day14

auto main() -> int {
  const auto robots = Day14::robotsFromFile("14/sample.txt");
  animate(robots, Utils::Coordinate{11, 7});
}

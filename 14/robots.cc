#include "robots.hh"

#include <fmt/core.h>

#include <filesystem>
#include <ranges>
#include <string>
#include <vector>

#include "external/ctre.hpp"
#include "utils/coordinate.hh"
#include "utils/nm_view.hh"
#include "utils/read_file.hh"

namespace Day14::Internal {

[[nodiscard]] auto buildRobot(const std::string& line) -> Robot {
  using namespace ctre::literals;  // NOLINT
  auto [_, px, py, vx, vy] =
      ctre::match<R"(p=(\d+),(\d+)\s+v=(-?\d+),(-?\d+))">(line);
  return Robot{.position = {px.to_number(), py.to_number()},
               .velocity = {vx.to_number(), vy.to_number()}};
}

}  // namespace Day14::Internal

namespace Day14 {

auto robotsFromFile(const std::filesystem::path& path) -> Robots {
  return Utils::readLines(path)                         //
         | std::views::transform(Internal::buildRobot)  //
         | std::ranges::to<std::vector>();
}

auto distanceBetween(const Robots& robots) -> double {
  if (robots.empty()) return 0;
  auto distance = double{};
  for (const auto [first, second] : Utils::nm_const_view(robots))
    distance += first->position.distanceFrom(second->position);
  return distance / static_cast<double>(robots.size() * robots.size());
}

auto distanceFrom(const Robots& robots, Utils::Coordinate where) -> double {
  if (robots.empty()) return 0;
  auto distance = double{};
  for (const auto& robot : robots) {
    distance += robot.position.distanceFrom(where);
  }
  return distance / static_cast<double>(robots.size());
}

}  // namespace Day14

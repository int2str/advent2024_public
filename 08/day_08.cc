//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/8
//

#include <array>
#include <fstream>
#include <ranges>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_set.hh"
#include "utils/curry.hh"
#include "utils/grid.hh"

namespace Day8 {

using AntennaGrid = Utils::Grid<char>;

[[nodiscard]] auto antiNodes(const AntennaGrid& grid) -> size_t {
  const auto is_antenna = [&](auto coordinate) {
    return grid[coordinate] != '.';
  };

  const auto is_same_frequency = [&](auto first, auto second) {
    return first != second and grid[first] == grid[second];
  };

  const auto antinodes_for = [&](auto first, auto second) {
    const auto distance = second - first;
    return std::array{first - distance, second + distance};
  };

  const auto in_bounds = [&](auto coordinate) {
    return grid.inBounds(coordinate);
  };

  auto antennae = grid.coordinates() | std::views::filter(is_antenna);

  const auto antinodes =
      std::views::cartesian_product(antennae, antennae)        //
      | std::views::filter(Utils::uncurry(is_same_frequency))  //
      | std::views::transform(Utils::uncurry(antinodes_for))   //
      | std::views::join                                       //
      | std::views::filter(in_bounds)                          //
      | std::ranges::to<Utils::CoordinateSet>();
  return antinodes.count();
}

[[nodiscard]] auto harmonicAntiNodes(const AntennaGrid& grid) -> size_t {
  const auto is_antenna = [&](auto coordinate) {
    return grid[coordinate] != '.';
  };

  const auto is_same_frequency = [&](auto first, auto second) {
    return first != second and grid[first] == grid[second];
  };

  const auto antinodes_for = [&](auto first, auto second) {
    auto nodes          = std::vector<Utils::Coordinate>{};
    const auto distance = second - first;
    while (grid.inBounds(first)) {
      nodes.push_back(first);
      first += distance;
    }
    while (grid.inBounds(second)) {
      nodes.push_back(second);
      second += distance;
    }
    return nodes;
  };

  auto antennae = grid.coordinates() | std::views::filter(is_antenna);

  const auto antinodes =
      std::views::cartesian_product(antennae, antennae)        //
      | std::views::filter(Utils::uncurry(is_same_frequency))  //
      | std::views::transform(Utils::uncurry(antinodes_for))   //
      | std::views::join                                       //
      | std::ranges::to<Utils::CoordinateSet>();
  return antinodes.count();
}

}  // namespace Day8

TEST(Day_08_Resonant_Collinearity_SAMPLE) {
  auto file       = std::ifstream("08/sample.txt");
  const auto grid = Day8::AntennaGrid::from(file);
  EXPECT_EQ(Day8::antiNodes(grid), 14);
  EXPECT_EQ(Day8::harmonicAntiNodes(grid), 34);
}

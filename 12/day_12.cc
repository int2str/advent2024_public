//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/12
//

#include <fmt/core.h>

#include <algorithm>  // IWYU pragma: keep
#include <filesystem>
#include <fstream>
#include <ranges>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_set.hh"
#include "utils/grid.hh"
#include "utils/sum.hh"

namespace Day12 {

using GardenGrid = Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<char{}>>;

[[nodiscard]] auto makeGrid(const std::filesystem::path& path) -> GardenGrid {
  auto file = std::ifstream(path);
  return GardenGrid::from(file);
}

void expandPatch(const GardenGrid& grid, Utils::Coordinate from,
                 Utils::CoordinateSet* patch, Utils::CoordinateSet* seen) {
  if (seen->contains(from)) return;
  seen->insert(from);
  patch->insert(from);
  const auto same = [&](auto to) { return grid[from] == grid[to]; };
  for (auto neighbor :
       from.neighborsUpDownLeftRight() | std::views::filter(same))
    expandPatch(grid, neighbor, patch, seen);
}

[[nodiscard]] auto patches(const GardenGrid& grid)
    -> std::vector<Utils::CoordinateSet> {
  auto seen         = Utils::CoordinateSet{};
  const auto unseen = [&](auto tile) { return !seen[tile]; };

  const auto make_patch = [&](auto from) {
    auto patch = Utils::CoordinateSet{};
    expandPatch(grid, from, &patch, &seen);
    return patch;
  };
  return grid.coordinates()                   //
         | std::views::filter(unseen)         //
         | std::views::transform(make_patch)  //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] auto perimeters(const GardenGrid& grid,
                              Utils::Coordinate from) -> size_t {
  const auto different = [&](auto to) { return grid[from] != grid[to]; };
  auto others = from.neighborsUpDownLeftRight() | std::views::filter(different);
  return static_cast<size_t>(
      std::ranges::distance(std::begin(others), std::end(others)));
}

[[nodiscard]] auto sides(const GardenGrid& grid,
                         Utils::Coordinate from) -> size_t {
  auto sides         = size_t{};
  const auto same_as = [&](auto other) {
    return grid[from + other] == grid[from];
  };
  const auto new_side = [&](auto directions1, auto direction2) {
    return !same_as(directions1) and
           (!same_as(direction2) or same_as(directions1 + direction2));
  };
  if (new_side(Utils::Direction::up(), Utils::Direction::left())) ++sides;
  if (new_side(Utils::Direction::down(), Utils::Direction::left())) ++sides;
  if (new_side(Utils::Direction::left(), Utils::Direction::up())) ++sides;
  if (new_side(Utils::Direction::right(), Utils::Direction::up())) ++sides;
  return sides;
}

[[nodiscard]] auto priceOf(const GardenGrid& grid, auto&& units) -> size_t {
  const auto patch_cost = [&](const auto& patch) {
    return patch.count() * Utils::sum(patch | std::views::transform(units));
  };
  return Utils::sum(patches(grid) | std::views::transform(patch_cost));
}

[[nodiscard]] auto priceOfFencing(const GardenGrid& grid) -> size_t {
  return priceOf(grid, [&](auto tile) { return perimeters(grid, tile); });
}

[[nodiscard]] auto discountedPrice(const GardenGrid& grid) -> size_t {
  return priceOf(grid, [&](auto tile) { return sides(grid, tile); });
}

}  // namespace Day12

TEST(Day_12_Garden_Groups_SAMPLE) {
  const auto grid = Day12::makeGrid("12/sample.txt");
  EXPECT_EQ(Day12::priceOfFencing(grid), 1930);
  EXPECT_EQ(Day12::discountedPrice(grid), 1206);
}

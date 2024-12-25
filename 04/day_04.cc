//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/4
//

#include <filesystem>
#include <fstream>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/coordinate_directions.hh"
#include "utils/curry.hh"
#include "utils/grid.hh"

namespace Day4 {

using XmasGrid = Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<char{}>>;

[[nodiscard]] auto makeGrid(const std::filesystem::path& path) -> XmasGrid {
  auto file = std::ifstream(path);
  return XmasGrid::from(file);
}

[[nodiscard]] constexpr auto find(const XmasGrid& grid,
                                  std::string_view word) -> int64_t {
  const auto find_rest = [&](this auto self, auto from, auto direction,
                             auto rest) -> bool {
    if (rest.empty()) return true;
    if (grid[from + direction] != rest.front()) return false;
    return self(from + direction, direction, rest.substr(1));
  };

  const auto find_first = [&](auto from, auto direction) -> bool {
    return find_rest(from, direction, std::string_view{"MAS"});
  };

  const auto is_first = [&](auto coordinate) {
    return grid[coordinate] == word.front();
  };

  auto found = std::views::cartesian_product(
                   grid.coordinates() | std::views::filter(is_first),
                   Utils::Directions::clockwise())  //
               | std::views::filter(Utils::uncurry(find_first));
  return std::distance(std::begin(found), std::end(found));
}

[[nodiscard]] auto x_mas(const XmasGrid& grid) -> int64_t {
  const auto is_A = [&](auto coordinate) { return grid[coordinate] == 'A'; };

  const auto is_MS = [&](auto from, auto direction) {
    return (grid[from + direction] == 'M' and grid[from - direction] == 'S') or
           (grid[from + direction] == 'S' and grid[from - direction] == 'M');
  };

  const auto is_MAS = [&](auto from) {
    return is_MS(from, Utils::Direction::upLeft()) and
           is_MS(from, Utils::Direction::upRight());
  };

  auto found = grid.coordinates()          //
               | std::views::filter(is_A)  //
               | std::views::filter(is_MAS);
  return std::distance(std::begin(found), std::end(found));
}

}  // namespace Day4

TEST(Day_04_Ceres_Search_SAMPLE) {
  const auto grid = Day4::makeGrid("04/sample.txt");
  EXPECT_EQ(Day4::find(grid, "XMAS"), 18);
  EXPECT_EQ(Day4::x_mas(grid), 9);
}

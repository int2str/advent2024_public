//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/18
//

#include <filesystem>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_map.hh"  // IWYU pragma: keep
#include "utils/dijkstras.hh"
#include "utils/grid.hh"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day18 {

using Chunks = std::vector<Utils::Coordinate>;
using Edge   = Utils::WeightedEdge<int, Utils::Coordinate>;
using Grid   = Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<'#'>>;

struct Map {
  Chunks chunks;
  size_t width;
};

[[nodiscard]] auto findEscapeLength(const Map& map,
                                    Chunks::const_iterator search_from) -> int {
  auto grid = Grid{map.width, map.width};
  std::for_each(map.chunks.cbegin(), search_from,
                [&](const auto& coordinate) { grid[coordinate] = '#'; });

  const auto start      = Utils::Coordinate(0, 0);
  const auto start_edge = Edge{0, start};
  const auto target     = Utils::Coordinate{static_cast<int>(grid.width()) - 1,
                                        static_cast<int>(grid.height()) - 1};

  const auto adjacent = [&](const auto& from) {
    return Utils::Directions::orthogonal()                                   //
           | std::views::transform([&](auto dir) { return from + dir; })     //
           | std::views::filter([&](auto pos) { return grid[pos] != '#'; })  //
           | std::views::transform([](auto pos) { return Edge{1, pos}; })    //
           | std::ranges::to<std::vector>();
  };

  return Utils::dijkstra<int, Utils::Coordinate>(start_edge, target, adjacent);
}

[[nodiscard]] auto readChunks(const std::filesystem::path& path) -> Chunks {
  const auto split = [](auto line) { return Utils::split<int, 2>(line, ","); };
  const auto to_coordinate = [](auto pair) {
    return Utils::Coordinate{pair[0], pair[1]};
  };
  return Utils::readLines(path)                  //
         | std::views::transform(split)          //
         | std::views::transform(to_coordinate)  //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] auto escape(
    const Map& map, Chunks::const_iterator::difference_type escape_at) -> int {
  return findEscapeLength(map, map.chunks.cbegin() + escape_at);
}

[[nodiscard]] auto trapped(const Map& map) -> Utils::Coordinate {
  auto low  = map.chunks.cbegin();
  auto high = map.chunks.cend();

  while (low < high) {
    const auto mid = low + (high - low) / 2;
    if (findEscapeLength(map, mid) == 0) {
      high = mid;
    } else {
      low = mid + 1;
    }
  }

  return *std::prev(low);
}

}  // namespace Day18

TEST(Day_18_RAM_Run_SAMPLE) {
  const auto map = Day18::Map{Day18::readChunks("18/sample.txt"), 7U};
  EXPECT_EQ(Day18::escape(map, 12), 22);
  EXPECT_EQ(Day18::trapped(map), Utils::Coordinate(6U, 1U));
}

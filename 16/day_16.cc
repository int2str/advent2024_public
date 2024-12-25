//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/16
//

#include <algorithm>  // IWYU pragma: keep
#include <filesystem>
#include <fstream>
#include <limits>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_set.hh"
#include "utils/coordinate_step.hh"
#include "utils/coordinate_step_map.hh"  // IWYU pragma: keep
#include "utils/dijkstras.hh"
#include "utils/grid.hh"

namespace Day16 {

using Map  = Utils::Grid<char>;
using Edge = Utils::WeightedEdge<int, Utils::Step>;

[[nodiscard]] auto loadMap(const std::filesystem::path& path) -> Map {
  auto map_file = std::ifstream(path);
  return Map::from(map_file);
}

[[nodiscard]] auto findPath(const Map& map) {
  const auto start = map.find('S').value_or(Utils::Coordinate{});
  const auto start_edge =
      Edge{0, Utils::Step{start, Utils::Direction::right()}};

  const auto adjacent = [&](const auto& from) {
    const auto edges = {
        Edge{1, {from.next(), from.direction}},
        {1000, {from.position, Utils::rotatedCounterClockwise(from.direction)}},
        {1000, {from.position, Utils::rotatedClockwise(from.direction)}}};
    const auto in_bounds = [&](auto to) {
      return map[to.distance == 1 ? from.next() : to.edge.next()] != '#';
    };
    return edges | std::views::filter(in_bounds) |
           std::ranges::to<std::vector>();
  };

  return Utils::dijkstra<int, Utils::Step>(start_edge, adjacent);
}

[[nodiscard]] constexpr auto bestSeats(Utils::Step finish,
                                       const auto& previous) -> size_t {
  auto stack   = std::vector<Utils::Step>{finish};
  auto visited = std::unordered_set<Utils::Step>{finish};
  auto unique  = Utils::CoordinateSet{finish.position};
  while (!stack.empty() and previous.contains(stack.front())) {
    const auto to = stack.front();
    stack.erase(stack.begin());

    for (const auto& previous_edge : previous.at(to)) {
      if (!visited.contains(previous_edge)) {
        visited.insert(previous_edge);
        stack.push_back(previous_edge);
        unique.insert(previous_edge.position);
      }
    }
  }
  return unique.count();
}

[[nodiscard]] auto runMaze(const Map& map) -> std::pair<int, size_t> {
  const auto [distances, previous] = findPath(map);

  const auto finish    = map.find('E').value_or(Utils::Coordinate{});
  const auto is_finish = [&](auto weighted) {
    return std::get<0>(weighted).position == finish;
  };

  const auto lowest = [](auto acc, auto edge) -> std::pair<Utils::Step, int> {
    if (edge.second < acc.second) return edge;
    return acc;
  };

  const auto init =
      std::make_pair(Utils::Step{}, std::numeric_limits<int>::max());

  auto finishes = distances | std::views::filter(is_finish);
  const auto [min_at, min_distance] =
      std::ranges::fold_left(finishes, init, lowest);
  return std::make_pair(min_distance, bestSeats(min_at, previous));
}

}  // namespace Day16

TEST(Day_16_Reindeer_Maze_SAMPLE) {
  const auto [distance, best_seats] =
      Day16::runMaze(Day16::loadMap("16/sample.txt"));
  EXPECT_EQ(distance, 11'048);
  EXPECT_EQ(best_seats, 64);
}

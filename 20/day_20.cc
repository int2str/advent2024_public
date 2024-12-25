//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/20
//

#include <fmt/core.h>

#include <filesystem>
#include <fstream>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_map.hh"  // IWYU pragma: keep
#include "utils/dijkstras.hh"
#include "utils/grid.hh"
#include "utils/nm_view.hh"

namespace Day20 {

using Map  = Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<'#'>>;
using Edge = Utils::WeightedEdge<int, Utils::Coordinate>;

using Route       = std::vector<Utils::Coordinate>;
using DistanceMap = std::array<int, 256ULL * 256>;

[[nodiscard]] constexpr auto at(Utils::Coordinate coordinate) -> size_t {
  return static_cast<size_t>(coordinate.y) * 256ULL +
         static_cast<size_t>(coordinate.x);
}

[[nodiscard]] auto loadMap(const std::filesystem::path& path) -> Map {
  auto file = std::ifstream(path);
  return Map::from(file);
}

[[nodiscard]] auto findPath(const Map& map) -> std::pair<DistanceMap, Route> {
  const auto end = map.find('E').value_or(Utils::Coordinate{});

  const auto adjacent = [&](const auto& from) {
    return Utils::Directions::orthogonal()                                  //
           | std::views::transform([&](auto dir) { return from + dir; })    //
           | std::views::filter([&](auto pos) { return map[pos] != '#'; })  //
           | std::views::transform([](auto pos) { return Edge{1, pos}; })   //
           | std::ranges::to<std::vector>();
  };

  const auto& [distances, previous] = Utils::dijkstra(Edge{0, end}, adjacent);

  auto distance_map = DistanceMap{};
  for (const auto [coordinate, distance] : distances)
    distance_map[at(coordinate)] = distance;
  distance_map[at(end)] = 0;

  auto route    = Route{};
  auto position = map.find('S').value_or(Utils::Coordinate{});
  while (true) {
    route.push_back(position);
    if (position == end) break;
    position = *previous.at(position).begin();
  }

  return std::make_pair(distance_map, route);
}

[[nodiscard]] auto findCheats(const Map& map,
                              int save_at_least) -> std::pair<size_t, size_t> {
  const auto [distance_map, route] = findPath(map);

  auto old_rules = int{};
  auto new_rules = int{};

  for (const auto [a, b] : Utils::nm_const_view(route)) {
    const auto distance = a->manhattanDistanceFrom(*b);
    if (distance <= 20) {
      const auto d1 = distance_map[at(*a)];
      const auto d2 = distance_map[at(*b)];
      if (d1 > d2                   //
          and (d1 - d2) > distance  //
          and (d1 - d2 - distance) >= save_at_least) {
        if (distance <= 2) ++old_rules;
        ++new_rules;
      }
    }
  }

  return {old_rules, new_rules};
}

}  // namespace Day20

TEST(Day_20_Race_Condition_SAMPLE) {
  const auto map            = Day20::loadMap("20/sample.txt");
  const auto [_, new_rules] = Day20::findCheats(map, 50U);
  EXPECT_EQ(new_rules, 285U);
}

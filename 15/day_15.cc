//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/15
//

#include <fstream>

#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/grid.hh"
#include "utils/one_of.hh"
#include "utils/read_file.hh"
#include "utils/sum.hh"

namespace Day15 {

using Utils::Coordinate;
using Utils::Direction;

using Map   = Utils::Grid<char>;
using Moves = std::vector<char>;

struct Instructions {
  Map map;
  Moves moves;
};

[[nodiscard]] auto readInstructions(const std::string& prefix) -> Instructions {
  auto map_file = std::ifstream(prefix + "_map.txt");
  return {.map   = Map::from(map_file),
          .moves = Utils::readFile(prefix + "_moves.txt")};
}

[[nodiscard]] constexpr auto directionFrom(char chr) -> Utils::Coordinate {
  // clang-format off
  switch (chr) {
    case '<': return Direction::left();
    case '>': return Direction::right();
    case '^': return Direction::up();
    case 'v': return Direction::down();
    default: return Coordinate{}; }
  // clang-format on
}

[[nodiscard]] auto doubleUp(const Map& in) -> Map {
  auto out = Map{in.width() * 2, in.height()};
  for (const auto at : in.coordinates()) {
    const auto out_at   = Coordinate{at.x * 2, at.y};
    const auto right_of = out_at + Direction::right();
    switch (in[at]) {
      case '#':
      case '.':
        out[out_at]   = in[at];
        out[right_of] = in[at];
        break;
      case 'O':
        out[out_at]   = '[';
        out[right_of] = ']';
        break;
      case '@':
        out[out_at]   = '@';
        out[right_of] = '.';
        break;
      default:
        break;
    }
  }
  return out;
}

[[nodiscard]] auto canMove(const Map& map, Coordinate from,
                           Coordinate direction) -> bool;

[[nodiscard]] auto canMoveBox(const Map& map, Coordinate from,
                              Coordinate direction) -> bool {
  const auto to1 = from + direction;
  const auto to2 =
      to1 + (map[from] == ']' ? Direction::left() : Direction::right());
  if (map[to1] == '#' or map[to2] == '#') return false;
  return (map[to1] == '.' or canMove(map, to1, direction)) and
         (map[to2] == '.' or canMove(map, to2, direction));
}

[[nodiscard]] auto canMove(const Map& map, Coordinate from,
                           Coordinate direction) -> bool {
  const auto to = from + direction;
  if (map[from] == one_of('[', ']') and
      direction == one_of(Direction::up(), Direction::down()))
    return canMoveBox(map, from, direction);
  if (map[to] == '#') return false;
  if (map[to] == '.') return true;
  return canMove(map, to, direction);
}

void moveLargeBox(Map& map, Coordinate from, const Coordinate direction) {
  if (map[from] == ']') from += Direction::left();
  if (map[from + direction] == one_of('[', ']'))
    moveLargeBox(map, from + direction, direction);

  const auto other = from + Direction::right();
  if (map[other + direction] == one_of('[', ']'))
    moveLargeBox(map, other + direction, direction);

  std::swap(map[from], map[from + direction]);
  std::swap(map[other], map[other + direction]);
}

constexpr auto move(Map& map, Coordinate from,
                    const Coordinate direction) -> Coordinate {
  const auto to = from + direction;
  if (map[to] == '#') return from;

  if ((map[from] == one_of('[', ']')) and
      (direction == Direction::up() or direction == Direction::down())) {
    if (!canMove(map, from, direction)) return from;
    moveLargeBox(map, from, direction);

  } else {
    if (map[to] == one_of('O', '[', ']')) move(map, to, direction);
  }

  if (map[to] == '.') {
    std::swap(map[to], map[from]);
    return to;
  }
  return from;
}

[[nodiscard]] auto moveRobots(Instructions& instructions) {
  auto robot = instructions.map.find('@').value_or(Utils::Coordinate{});
  for (const auto& step : instructions.moves) {
    if (const auto direction = directionFrom(step); !direction.isZero()) {
      if (canMove(instructions.map, robot, direction))
        robot = move(instructions.map, robot, direction);
    }
  }
}

[[nodiscard]] constexpr auto gpsScore(const Map& map) -> int {
  const auto is_box    = [&](auto at) { return map[at] == one_of('O', '['); };
  const auto gps_score = [](auto at) { return 100 * at.y + at.x; };
  return Utils::sum(map.coordinates()             //
                    | std::views::filter(is_box)  //
                    | std::views::transform(gps_score));
}

[[nodiscard]] auto warehouseOneScore(Instructions instructions) -> int {
  moveRobots(instructions);
  return gpsScore(instructions.map);
}

[[nodiscard]] auto warehouseTwoScore(Instructions instructions) -> int {
  instructions.map = doubleUp(instructions.map);
  moveRobots(instructions);
  return gpsScore(instructions.map);
}

}  // namespace Day15

TEST(Day_15_Warehouse_Woes_SAMPLE) {
  const auto instructions = Day15::readInstructions("15/sample");
  EXPECT_EQ(Day15::warehouseOneScore(instructions), 10092);
  EXPECT_EQ(Day15::warehouseTwoScore(instructions), 9021);
}

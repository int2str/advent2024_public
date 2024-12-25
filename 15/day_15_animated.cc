//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/14
//

#include <fmt/core.h>

#include <fstream>

#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/grid.hh"
#include "utils/read_file.hh"
#include "window.hh"

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

constexpr auto move(Map& map, Coordinate from,
                    const Coordinate direction) -> Coordinate {
  const auto to = from + direction;
  if (map[to] == '#') return from;
  if (map[to] == 'O') move(map, to, direction);
  if (map[to] == '.') {
    std::swap(map[to], map[from]);
    return to;
  }
  return from;
}

void animate(Instructions instructions) {
  auto window = Window(Coordinate{static_cast<int>(instructions.map.width()),
                                  static_cast<int>(instructions.map.height())});
  window.fps(60);

  auto robot     = instructions.map.find('@').value_or(Utils::Coordinate{});
  auto direction = Direction::up();
  auto move_it   = instructions.moves.begin();

  while (window.isOpen()) {
    window.handleEvents();
    const auto move_completed = window.draw(instructions.map, robot, direction);

    if (move_completed and move_it != instructions.moves.end()) {
      fmt::print("{}\n", *move_it);
      direction = directionFrom(*move_it++);
      robot     = move(instructions.map, robot, direction);
    }
  }
}

}  // namespace Day15

auto main(int argc, char** argv) -> int {
  const auto instructions =
      Day15::readInstructions(argc < 2 ? "15/sample" : argv[1]);
  Day15::animate(instructions);
}

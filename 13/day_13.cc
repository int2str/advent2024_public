//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/13
//

#include <ranges>
#include <vector>

#include "external/ctre.hpp"
#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/read_file.hh"
#include "utils/sum.hh"

namespace Day13 {

using Coordinate = Utils::CoordinateBase<int64_t>;

struct Machine {
  Coordinate button_a{};
  Coordinate button_b{};
  Coordinate prize{};

  [[nodiscard]] static constexpr auto fix(Machine machine) -> Machine {
    machine.prize = machine.prize + 10'000'000'000'000;
    return machine;
  };
};

using Machines = std::vector<Machine>;

[[nodiscard]] auto loadConfig(const std::filesystem::path& path) -> Machines {
  using namespace ctre::literals;  // NOLINT
  auto machines   = Machines{};
  const auto file = Utils::readFile(path);
  for (auto [matched, ax, ay, bx, by, px, py] :
       ctre::search_all<R"(Button A: X\+(\d+), Y\+(\d+)\s?)"
                        R"(Button B: X\+(\d+), Y\+(\d+)\s?)"
                        R"(Prize: X=(\d+), Y=(\d+))">(file)) {
    if (matched) {
      machines.push_back(
          Machine{.button_a = Coordinate{ax.to_number(), ay.to_number()},
                  .button_b = Coordinate{bx.to_number(), by.to_number()},
                  .prize    = Coordinate{px.to_number(), py.to_number()}});
    }
  }
  return machines;
}

[[nodiscard]] constexpr auto B(Machine machine) -> int64_t {
  // B = (K2 * X1 - K1 * Y1) / (X1 * Y2 - X2 * Y1)
  return (machine.prize.y * machine.button_a.x -
          machine.prize.x * machine.button_a.y) /
         (machine.button_a.x * machine.button_b.y -
          machine.button_b.x * machine.button_a.y);
}

[[nodiscard]] constexpr auto AgivenB(Machine machine, int64_t B) -> int64_t {
  // A = (K1 - X2 * B) / X1
  return (machine.prize.x - machine.button_b.x * B) / machine.button_a.x;
}

[[nodiscard]] auto tokens(const Machine& machine) -> int64_t {
  const auto b = B(machine);
  const auto a = AgivenB(machine, b);
  if ((machine.button_a * a + machine.button_b * b) == machine.prize)
    return a * 3 + b;
  return 0;
}

[[nodiscard]] auto totalTokens(const Machines& machines) -> int64_t {
  return Utils::sum(machines  //
                    | std::views::transform(tokens));
}

[[nodiscard]] auto correctedTokens(const Machines& machines) -> int64_t {
  return Utils::sum(machines                               //
                    | std::views::transform(Machine::fix)  //
                    | std::views::transform(tokens));
}

}  // namespace Day13

TEST(Day_13_Claw_Contraption_SAMPLE) {
  const auto machines = Day13::loadConfig("13/sample.txt");
  EXPECT_EQ(Day13::totalTokens(machines), 480);
  // No given solution for part 2; recording calculated value here...
  EXPECT_EQ(Day13::correctedTokens(machines), 875'318'608'908);
}

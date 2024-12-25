//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/17
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <array>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include "day_17_vm.hh"
#include "external/ctre.hpp"
#include "testrunner/testrunner.h"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day17 {

struct Program {
  std::array<uint64_t, 3> registers{};
  std::vector<uint8_t> instructions{};
};

[[nodiscard]] auto readProgram(const std::filesystem::path& path) -> Program {
  using namespace ctre::literals;           // NOLINT
  const auto file = Utils::readLines(path)  //
                    | std::views::join      //
                    | std::ranges::to<std::string>();
  auto [_, ra, program] = ctre::match<R"(Register A: (\d+).*)"
                                      R"(Program: ([\d,]+))">(file);
  return Program{.registers    = {static_cast<uint64_t>(ra.to_number()), 0, 0},
                 .instructions = Utils::split<uint8_t>(program, ",")};
}

[[nodiscard]] auto runProgram(const Program& program) -> std::string {
  auto* compiled    = jit(program.instructions);
  const auto result = compiled(program.registers[0], 0U, 0U);
  return fmt::format("{}", fmt::join(decode3Bit(result), ","));
}

[[nodiscard]] auto findQuine(const Program& program) -> uint64_t {
  const auto pgm3b = encode3Bit(program.instructions);
  auto mask        = size_t{0x3F};
  auto try_ra      = uint64_t{1};

  auto* compiled = jit(program.instructions);

  while (true) {
    const auto result = compiled(try_ra, 0U, 0U);
    if (result == pgm3b) return try_ra;

    if ((result & mask) == (pgm3b & mask)) {
      mask = (mask << 3) + 0x7;
      try_ra <<= 3;

    } else {
      ++try_ra;
    }

    if (mask == 0x3F and try_ra > mask) break;
  }

  return 0U;
}

}  // namespace Day17

TEST(Day_17_Chronospatial_Computer_SAMPLE) {
  const auto program = Day17::readProgram("17/sample.txt");
  EXPECT_EQ(Day17::runProgram(program), "4,6,3,5,6,3,5,2,1,0");
  EXPECT_EQ(Day17::findQuine(program), 0);
}

TEST(Day_17_Chronospatial_Computer_QUINE) {
  const auto program = Day17::readProgram("17/quine.txt");
  EXPECT_EQ(Day17::runProgram(program), "0,3,5,4,3,0");
  EXPECT_EQ(Day17::findQuine(program), 117'440ULL);
}

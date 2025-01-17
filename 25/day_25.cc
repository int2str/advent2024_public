//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/25
//

#include <bitset>
#include <filesystem>
#include <ranges>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/curry.hh"
#include "utils/nm_pairs.hh"
#include "utils/read_file.hh"
#include "utils/sum.hh"

namespace Day25 {

// clang-format off

[[nodiscard]] auto readLocksAndKeys(const std::filesystem::path& path)
    -> size_t {
  const auto pins = Utils::readLines(path)
      | std::views::join
      | std::views::transform([](auto ch) { return ch == '#' ? '1' : '0'; })
      | std::views::chunk(35)
      | std::views::transform([](auto chunk) {
          return std::bitset<35>{chunk | std::ranges::to<std::string>()}; })
      | std::ranges::to<std::vector>();

  return Utils::sum(
        pins
      | Utils::views::nm_pairs
      | std::views::transform(Utils::uncurry([](auto a, auto b) -> size_t {
          return (a & b) == 0; })));
}

// clang-format on

}  // namespace Day25

TEST(Day_25_Code_Chronicle_SAMPLE) {
  EXPECT_EQ(Day25::readLocksAndKeys("25/sample.txt"), 3);
}

//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/25
//

#include <bitset>
#include <filesystem>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/nm_view.hh"
#include "utils/read_file.hh"

namespace Day25 {

// clang-format off

[[nodiscard]] auto readLocksAndKeys(const std::filesystem::path& path)
    -> size_t {
  auto pins = Utils::readLines(path)
      | std::views::join
      | std::views::transform([](auto ch) { return ch == '#' ? '1' : '0'; })
      | std::views::chunk(35)
      | std::views::transform([](auto chunk) {
          return std::bitset<35>{chunk | std::ranges::to<std::string>()}; })
      | std::ranges::to<std::vector>();

  auto matches = size_t{};
  for (const auto [a, b] : Utils::nm_const_view(pins))
    matches += size_t{(*a & *b) == 0};
  return matches;
}

// clang-format on

}  // namespace Day25

TEST(Day_25_Code_Chronicle_SAMPLE) {
  EXPECT_EQ(Day25::readLocksAndKeys("25/sample.txt"), 3);
}

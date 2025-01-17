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
  const auto pins =
      // Read lines; strip EOL
        Utils::readLines(path)
      | std::views::join

      // Turn '#'s into '1's, '.'s into '0's
      | std::views::transform([](auto ch) { return ch == '#' ? '1' : '0'; })

      // Iterate 35 characters (Will all be '1's and '0's) at a time
      | std::views::chunk(35)

      // Turn the "101010111..." strings into a std::bitset
      | std::views::transform([](auto chunk) {
          return std::bitset<35>{chunk | std::ranges::to<std::string>()}; })

      // Return a vector of these bitsets
      | std::ranges::to<std::vector>();

    
  // Return the sum of...
  return Utils::sum(
      // Iterate the bitmasks
        pins

      // Process then in "N * M" pairs (like bubble sort iterator basicaaly)
      | Utils::views::nm_pairs

      // AND them together and turn the bitset pairs
      // into a 1 for a match or 0 foir no match
      | std::views::transform(Utils::uncurry([](auto a, auto b) -> size_t {
          return (a & b) == 0; })));

  // Done :)
}

// clang-format on

}  // namespace Day25

TEST(Day_25_Code_Chronicle_SAMPLE) {
  EXPECT_EQ(Day25::readLocksAndKeys("25/sample.txt"), 3);
}

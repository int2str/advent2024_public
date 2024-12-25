//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/19
//

#include <filesystem>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/read_file.hh"
#include "utils/split.hh"
#include "utils/sum.hh"

namespace Day19 {

using Towels   = std::vector<std::string>;
using Patterns = std::vector<std::string>;

[[nodiscard]] auto loadRules(const std::filesystem::path& path)
    -> std::pair<Towels, Patterns> {
  const auto lines = Utils::readLines(path);
  return std::make_pair(
      Utils::str_split(lines.front(), ", ") | std::ranges::to<std::vector>(),
      lines | std::views::drop(2) | std::ranges::to<std::vector>());
}

[[nodiscard]] auto match(auto& cache, const std::string& pattern,
                         const Towels& towels) -> size_t {
  if (pattern.empty()) return 1U;
  if (cache.contains(pattern)) return cache.at(pattern);

  auto matches = size_t{};
  for (const auto& towel : towels) {
    if (pattern.starts_with(towel))
      matches += match(cache, pattern.substr(towel.length()), towels);
  }

  cache[pattern] = matches;
  return matches;
}

[[nodiscard]] auto countDesigns(const Towels& towels, const Patterns& patterns)
    -> std::pair<size_t, size_t> {
  auto cache       = std::unordered_map<std::string, size_t>{};
  const auto count = [&](auto pattern) {
    return match(cache, pattern, towels);
  };
  const auto combinations =
      patterns                                                     //
      | std::views::transform(count)                               //
      | std::views::filter([](auto count) { return count != 0; })  //
      | std::ranges::to<std::vector>();

  return std::make_pair(combinations.size(), Utils::sum(combinations));
}

}  // namespace Day19

TEST(Day_19_Linen_Layout_SAMPLE) {
  const auto [towels, patterns] = Day19::loadRules("19/sample.txt");
  const auto [unique, total]    = Day19::countDesigns(towels, patterns);
  EXPECT_EQ(unique, 6);
  EXPECT_EQ(total, 16);
}

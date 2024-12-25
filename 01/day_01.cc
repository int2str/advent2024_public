//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/1
//

#include <algorithm>
#include <numeric>
#include <ranges>
#include <unordered_map>

#include "testrunner/testrunner.h"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day1 {

[[nodiscard]] auto readListsSorted(const std::filesystem::path& path)
    -> std::pair<std::vector<int>, std::vector<int>> {
  auto left  = std::vector<int>{};
  auto right = std::vector<int>{};

  const auto values = Utils::readLines(path)  //
                      | std::views::transform([](const auto& line) {
                          return Utils::split<int, 2>(line, "   ");
                        });
  for (const auto& [left_, right_] : values) {
    left.push_back(left_);
    right.push_back(right_);
  }

  // Part 1 requires sorting; Part 2 doesn't care...
  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  return {left, right};
}

[[nodiscard]] auto totalDistance(std::vector<int>& first,
                                 std::vector<int>& second) -> int {
  constexpr auto abs_distance = [](int a, int b) { return std::abs(a - b); };

  const auto range = std::views::zip_transform(abs_distance, first, second);
  return std::accumulate(std::begin(range), std::end(range), int{});
}

[[nodiscard]] auto similarityScore(const std::vector<int>& first,
                                   const std::vector<int>& second) -> int {
  auto map = std::unordered_map<int, int>{};
  for (const auto& right : second) ++map[right];

  auto weighted = [&map](auto lookup) {
    return lookup * (map.contains(lookup) ? map[lookup] : int{});
  };

  const auto range = first | std::views::transform(weighted);
  return std::accumulate(std::begin(range), std::end(range), int{});
}

}  // namespace Day1

TEST(Day_01_Historian_Hysteria_SAMPLE) {
  auto [first, second] = Day1::readListsSorted("01/sample.txt");
  EXPECT_EQ(Day1::totalDistance(first, second), 11);
  EXPECT_EQ(Day1::similarityScore(first, second), 31);
}

//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/2
//

#include <algorithm>
#include <cmath>
#include <functional>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day2 {

[[nodiscard]] auto readReports(const std::filesystem::path& path) {
  return Utils::readLines(path)  //
         | std::views::transform([](const auto& line) {
             return Utils::split<int>(line, " ");
           })  //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] constexpr auto minmaxDifference(const std::vector<int>& record)
    -> std::ranges::minmax_result<int> {
  return std::ranges::minmax(record |
                             std::views::pairwise_transform(std::minus<int>{}));
}

[[nodiscard]] constexpr auto isSafe(std::ranges::minmax_result<int> minmax)
    -> bool {
  return minmax.min != 0 and minmax.max != 0  //
         and std::abs(minmax.min) <= 3        //
         and std::abs(minmax.max) <= 3        //
         and std::signbit(minmax.min) == std::signbit(minmax.max);
}

[[nodiscard]] auto safeReports(const auto& records) {
  const auto validated = records                                    //
                         | std::views::transform(minmaxDifference)  //
                         | std::views::transform(isSafe);
  return std::ranges::count(validated, true);
}

[[nodiscard]] auto safeReportsWithTolerance(const auto& records) {
  constexpr auto validate = [](auto record) -> bool {
    for (int64_t skip_idx = 0; skip_idx != static_cast<int64_t>(record.size());
         ++skip_idx) {
      auto copy = record;
      copy.erase(copy.begin() + skip_idx);
      const auto valid = isSafe(minmaxDifference(copy));
      if (valid) return true;
    }
    return false;
  };

  const auto validated = records | std::views::transform(validate);
  return std::ranges::count(validated, true);
}

}  // namespace Day2

TEST(Day_02_RedNosed_Reports_SAMPLE) {
  const auto reports = Day2::readReports("02/sample.txt");
  EXPECT_EQ(Day2::safeReports(reports), 2);
  EXPECT_EQ(Day2::safeReportsWithTolerance(reports), 4);
}

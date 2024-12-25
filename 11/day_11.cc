//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/11
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <cmath>
#include <string_view>
#include <unordered_map>

#include "testrunner/testrunner.h"
#include "utils/charconv.hh"
#include "utils/read_file.hh"
#include "utils/split.hh"

namespace Day11 {

template <typename T>
[[nodiscard]] constexpr auto countDigits(T number) -> uint8_t {
  if (number < 10) return 1;
  auto count = uint8_t{};
  while (number != 0) {
    number /= 10;
    ++count;
  }
  return count;
}

using StoneCount = std::unordered_map<uint64_t, size_t>;

[[nodiscard]] auto countsFromFile(const std::filesystem::path& path)
    -> StoneCount {
  const auto stones =
      Utils::split<uint64_t>(Utils::readLines(path).front(), " ");
  auto counts = StoneCount{};
  for (const auto& stone : stones) ++counts[stone];
  return counts;
}

[[nodiscard]] constexpr auto split(uint64_t number)
    -> std::pair<uint64_t, uint64_t> {
  const auto chars = std::string_view{fmt::format("{}", number)};
  return {Utils::from_chars<uint64_t>(chars.substr(0, chars.length() / 2)),
          Utils::from_chars<uint64_t>(chars.substr(chars.length() / 2))};
}

[[nodiscard]] auto transformStones(StoneCount counts, size_t blinks) -> size_t {
  auto buffer = StoneCount{};
  auto& in    = counts;
  auto& out   = buffer;

  while (blinks-- != 0) {
    out.clear();

    for (const auto& [id, count] : in) {
      if (id == 0) {
        out[1] += count;

      } else if ((countDigits(id) & 0x1) == 0) {
        const auto [a, b] = split(id);
        out[a] += count;
        out[b] += count;

      } else {
        out[id * 2024] += count;
      }
    }

    std::swap(in, out);
  }

  return std::ranges::fold_left(in, 0U, [](auto counts, const auto& tuple) {
    return counts + std::get<1>(tuple);
  });
}

}  // namespace Day11

TEST(Day_11_Plutonian_Pebbles_SAMPLE) {
  const auto counts = Day11::countsFromFile("11/sample.txt");
  EXPECT_EQ(Day11::transformStones(counts, 6), 22);
  EXPECT_EQ(Day11::transformStones(counts, 25), 55312);
}

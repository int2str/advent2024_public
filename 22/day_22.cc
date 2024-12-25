//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/22
//

#include <fmt/core.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <filesystem>
#include <ranges>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/charconv.hh"
#include "utils/read_file.hh"
#include "utils/sum.hh"

namespace Day22 {

using Seeds       = std::vector<int64_t>;
using Sequence    = std::array<int8_t, 4>;
using CountsArray = std::array<size_t, 160'000>;

[[nodiscard]] auto readSeeds(const std::filesystem::path& path)
    -> std::vector<int64_t> {
  return Utils::readLines(path) |
         std::views::transform(Utils::from_chars<int64_t>) |
         std::ranges::to<std::vector>();
}

[[nodiscard]] constexpr auto mix(int64_t number, int64_t mixin) -> int64_t {
  return number ^ mixin;
}

[[nodiscard]] constexpr auto prune(int64_t number) -> int64_t {
  return (number % 16'777'216U);
}

[[nodiscard]] constexpr auto next(int64_t seed) -> int64_t {
  seed = prune(mix(seed, seed * 64U));
  seed = prune(mix(seed, seed / 32U));
  return prune(mix(seed, seed * 2048));
}

[[nodiscard]] constexpr auto idxFor(const Sequence& sequence) noexcept
    -> size_t {
  return (static_cast<size_t>(sequence[0] + 10) * 8'000) +
         (static_cast<size_t>(sequence[1] + 10) * 400) +
         (static_cast<size_t>(sequence[2] + 10) * 20) +
         static_cast<size_t>(sequence[3] + 10);
}

void sequenceBuyer(CountsArray& map, int64_t seed) {
  auto sequence = Sequence{};
  auto last     = static_cast<int8_t>(seed % 10);
  auto seen     = std::bitset<160'000>{};

  for (int64_t i = 0; i != 2'000; ++i) {
    seed = next(seed);

    std::rotate(sequence.rbegin(), sequence.rbegin() + 1, sequence.rend());
    const auto bananas = static_cast<int8_t>(seed % 10);
    sequence[0]        = static_cast<int8_t>(bananas - last);

    if (i > 3 and bananas != 0 and !seen.test(idxFor(sequence))) {
      map[idxFor(sequence)] += static_cast<size_t>(bananas);
      seen.set(idxFor(sequence));
    }
    last = bananas;
  }
}

[[nodiscard]] auto sequenceBuyers(const Seeds& seeds) -> size_t {
  auto total = CountsArray{};
  for (const auto seed : seeds) sequenceBuyer(total, seed);
  return std::ranges::max(total);
}

[[nodiscard]] auto buyersSecretSum(const Seeds& seeds) -> int64_t {
  constexpr auto gen_2k = [](auto seed) {
    for (int64_t i = 0; i != 2'000U; ++i) seed = next(seed);
    return seed;
  };
  return Utils::sum(seeds | std::views::transform(gen_2k));
}

}  // namespace Day22

TEST(Day_22_Monkey_Market_SAMPLE) {
  const auto seeds_sample = Day22::readSeeds("22/sample.txt");
  EXPECT_EQ(Day22::buyersSecretSum(seeds_sample), 37327623ULL);

  const auto seeds_sample2 = Day22::readSeeds("22/sample2.txt");
  EXPECT_EQ(Day22::sequenceBuyers(seeds_sample2), 23);
}

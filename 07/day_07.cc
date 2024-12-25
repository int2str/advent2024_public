//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/7
//

#include <fmt/core.h>

#include <algorithm>  // IWYU pragma: keep
#include <array>
#include <ranges>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/read_file.hh"
#include "utils/split.hh"
#include "utils/sum.hh"

namespace Day7 {

using Equation  = std::vector<uint64_t>;
using Equations = std::vector<Equation>;

[[nodiscard]] auto calibrationEquations(const std::filesystem::path& path)
    -> Equations {
  return Utils::readLines(path)  //
         | std::views::transform([](const auto& line) {
             return Utils::split<uint64_t>(line, " ");
           })  //
         | std::ranges::to<std::vector>();
}

// NOLINTNEXTLINE
[[nodiscard]] constexpr auto Confabulate(uint64_t a, uint64_t b) -> uint64_t {
  if (b == 0) return a * 10ULL;
  auto digits = std::array<uint8_t, 20>{};  // Max digits in U64
  auto* digit = digits.begin();
  while (b != 0) {
    *digit++ = static_cast<uint8_t>(b % 10);
    b /= 10ULL;
  }

  return std::ranges::fold_left(
      std::ranges::subrange(digits.begin(), digit) | std::views::reverse, a,
      [](auto acc, auto dig) { return acc * 10ULL + dig; });
}

template <typename... Ts>
[[nodiscard]] constexpr auto tryOp(uint64_t solution, uint64_t accumulated,
                                   auto operands, const auto& op,
                                   const std::tuple<Ts...>& ops) -> bool {
  accumulated = op(accumulated, operands.front());
  if (operands.size() == 1) return accumulated == solution;
  return (
      tryOp(solution, accumulated, operands.next(), std::get<Ts>(ops), ops) or
      ...);
}

template <typename... Ts>
[[nodiscard]] constexpr auto tryOps(const Equation& problem,
                                    const std::tuple<Ts...>& ops) -> bool {
  if (problem.size() < 3) return false;

  const auto solution    = problem.front();
  const auto accumulated = *std::next(problem.begin());
  const auto operands =
      std::ranges::subrange(problem.begin() + 2, problem.end());

  return (tryOp(solution, accumulated, operands, std::get<Ts>(ops), ops) or
          ...);
}

[[nodiscard]] auto fixPlusOrMultiplies(const Equation& problem) -> uint64_t {
  const auto ops = std::tuple(std::plus{}, std::multiplies{});
  return tryOps(problem, ops) ? problem.front() : 0U;
}

[[nodiscard]] auto alsoFixConcatenate(const Equation& problem) -> uint64_t {
  const auto ops = std::tuple(std::plus{}, std::multiplies{}, &Confabulate);
  return tryOps(problem, ops) ? problem.front() : 0U;
}

[[nodiscard]] auto calibrate(const Equations& problems,
                             const auto& fn) -> uint64_t {
  return Utils::sum(problems | std::views::transform(fn));
}

}  // namespace Day7

TEST(Day_07_Bridge_Repair_SAMPLE) {
  const auto equations = Day7::calibrationEquations("07/sample.txt");
  EXPECT_EQ(Day7::calibrate(equations, Day7::fixPlusOrMultiplies), 3749);
  EXPECT_EQ(Day7::calibrate(equations, Day7::alsoFixConcatenate), 11387);
}

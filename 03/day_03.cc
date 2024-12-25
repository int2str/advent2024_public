//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/3
//

#include <algorithm>
#include <cctype>
#include <optional>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/read_file.hh"

namespace Day3 {

using namespace std::literals;
constexpr auto MUL_OP         = "mul("sv;
constexpr auto DO_DIRECTIVE   = "do()"sv;
constexpr auto DONT_DIRECTIVE = "don't()"sv;

struct Op {
  std::vector<int> operands{};
  std::string_view* chars;
};

[[nodiscard]] constexpr auto find(std::string_view what,
                                  Op op) -> std::optional<Op> {
  const auto found_at = op.chars->find(what);
  op.chars->remove_prefix(
      std::min(found_at + what.length(), op.chars->length()));
  if (found_at == std::string_view::npos) return std::nullopt;
  return op;
}

[[nodiscard]] constexpr auto extractNumber(Op op) -> std::optional<Op> {
  if (op.chars->empty() or (std::isdigit(op.chars->at(0)) == 0))
    return std::nullopt;
  auto number = int{};
  for (size_t digits = 0; digits != 3; ++digits) {
    if (std::isdigit(op.chars->at(0)) == 0) break;
    number *= 10;  // NOLINT; 10 is not magic here ;)
    number += op.chars->at(0) - '0';
    op.chars->remove_prefix(1);
  }
  op.operands.push_back(number);
  return op;
}

template <char WHAT>
[[nodiscard]] constexpr auto expect(Op op) -> std::optional<Op> {
  if (op.chars->empty() or op.chars->at(0) != WHAT) return std::nullopt;
  op.chars->remove_prefix(1);
  return op;
}

[[nodiscard]] constexpr auto multiply(Op op) -> std::optional<int> {
  if (op.operands.size() != 2) return std::nullopt;
  return op.operands.at(0) * op.operands.at(1);
}

[[nodiscard]] constexpr auto nextOp(std::string_view* chars)
    -> std::optional<int> {
  return find(MUL_OP, {.chars = chars})
      .and_then(extractNumber)
      .and_then(expect<','>)
      .and_then(extractNumber)
      .and_then(expect<')'>)
      .and_then(multiply);
}

[[nodiscard]] constexpr auto parseGibberish(std::string_view chars) -> int {
  auto total = int{};
  while (!chars.empty()) {
    if (const auto op = Day3::nextOp(&chars)) total += *op;
  }
  return total;
}

[[nodiscard]] constexpr auto removeDisabledParts(std::string_view chars)
    -> std::string {
  auto str = std::string{chars};

  auto dont_pos = str.find(DONT_DIRECTIVE);
  while (dont_pos != std::string::npos) {
    auto until = str.find(DO_DIRECTIVE, dont_pos);
    if (until != std::string::npos) until -= (dont_pos - DO_DIRECTIVE.length());
    str.erase(dont_pos, until);
    dont_pos = str.find(DONT_DIRECTIVE);
  }

  return str;
}

[[nodiscard]] constexpr auto parseGibberishConditionally(
    std::string_view from_file) -> int {
  // We could have used regular expressions to parse the string....
  // We could have tokenized it and then parsed it properly....
  // But no...
  // We're just brute force removing the "don't()" parts ;)
  return parseGibberish(removeDisabledParts(std::string_view{from_file}));
}

}  // namespace Day3

TEST(Day_03_Mull_It_Over_SAMPLE) {
  const auto file = Utils::readFile("03/sample.txt");
  EXPECT_EQ(Day3::parseGibberish(std::string_view{file}), 161);
  EXPECT_EQ(Day3::parseGibberishConditionally(std::string_view{file}), 48);
}

//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/5
//

#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "testrunner/testrunner.h"
#include "utils/nm_view.hh"
#include "utils/read_file.hh"
#include "utils/split.hh"
#include "utils/sum.hh"

namespace Day5 {

using RuleMap = std::unordered_map<int, std::unordered_set<int>>;
using Pages   = std::vector<int>;
using Manuals = std::vector<Pages>;

[[nodiscard]] auto makeRuleMap(const std::filesystem::path& path) -> RuleMap {
  const auto split_rule = [](const auto& line) constexpr {
    return Utils::split<int, 2>(line, "|");
  };
  const auto rules = Utils::readLines(path)  //
                     | std::views::transform(split_rule);
  auto rule_map = RuleMap{};
  for (const auto [before, after] : rules) rule_map[before].insert(after);
  return rule_map;
}

[[nodiscard]] auto makePages(const std::filesystem::path& path) -> Manuals {
  const auto split_pages = [](const auto& line) constexpr {
    return Utils::split<int>(line, ",");
  };
  return Utils::readLines(path)                //
         | std::views::transform(split_pages)  //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] constexpr auto midpoint(const Pages& pages) -> int {
  return pages.at(pages.size() / 2);
};

[[nodiscard]] auto isValid(const RuleMap& rules, const Pages& pages) -> bool {
  auto seen = std::unordered_set<int>{};
  for (const auto& page : pages) {
    if (rules.contains(page)) {
      for (const auto& maybe_invalid : rules.at(page)) {
        if (seen.contains(maybe_invalid)) return false;
      }
    }
    seen.insert(page);
  }
  return true;
}

[[nodiscard]] auto validMiddlePageSum(const RuleMap& rules,
                                      const Manuals& manuals) -> int {
  const auto is_valid = [&](const auto& pages) constexpr {
    return isValid(rules, pages);
  };

  return Utils::sum(manuals                         //
                    | std::views::filter(is_valid)  //
                    | std::views::transform(midpoint));
}

[[nodiscard]] auto reorderInvalidPages(const RuleMap& rules,
                                       const Manuals& manuals) -> int {
  const auto is_invalid = [&](const auto& pages) constexpr {
    return !isValid(rules, pages);
  };

  const auto reorder = [&](auto pages) {
    for (auto [before, after] : Utils::nm_view(pages)) {
      if (rules.contains(*after) and rules.at(*after).contains(*before))
        std::swap(*before, *after);
    }
    return pages;
  };

  return Utils::sum(manuals                           //
                    | std::views::filter(is_invalid)  //
                    | std::views::transform(reorder)  //
                    | std::views::transform(midpoint));
}

}  // namespace Day5

TEST(Day_05_Print_Queue_SAMPLE) {
  const auto rules   = Day5::makeRuleMap("05/sample_rules.txt");
  const auto manuals = Day5::makePages("05/sample_pages.txt");
  EXPECT_EQ(Day5::validMiddlePageSum(rules, manuals), 143);
  EXPECT_EQ(Day5::reorderInvalidPages(rules, manuals), 123);
}

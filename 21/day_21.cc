//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/21
//

#include <fmt/core.h>

#include <limits>
#include <queue>
#include <ranges>

#include "testrunner/testrunner.h"
#include "utils/charconv.hh"
#include "utils/coordinate.hh"
#include "utils/curry.hh"
#include "utils/grid.hh"
#include "utils/read_file.hh"
#include "utils/sum.hh"
#include "utils/vector_cartesian_product.hh"

namespace std {

template <>
struct hash<std::array<char, 2>> {
  [[nodiscard]] auto operator()(
      const std::array<char, 2>& from_to) const noexcept -> size_t {
    return static_cast<size_t>(from_to[0]) * 1'024 +
           static_cast<size_t>(from_to[1]);
  }
};

}  // namespace std

namespace Day21 {

using Keypad = Utils::Grid<char, Utils::OutOfBoundsPolicy::Default<' '>>;
using Paths = std::unordered_map<std::array<char, 2>, std::vector<std::string>>;

[[nodiscard]] auto pathsForKeypad(const Keypad& keypad) -> Paths {
  auto paths = Paths{};

  auto non_empty =
      keypad.coordinates() | std::views::filter([&](auto coordinate) {
        return keypad[coordinate] != ' ';
      });

  for (const auto [from, to] :
       std::views::cartesian_product(non_empty, non_empty)) {
    if (from == to) {
      paths[{keypad[from], keypad[from]}].emplace_back("A");
      continue;
    }
    auto shortest_length = std::numeric_limits<size_t>::max();

    auto search = std::queue<std::pair<Utils::Coordinate, std::string>>{};
    search.emplace(from, "");

    auto beyond_shortest_length = false;
    while (!search.empty() and !beyond_shortest_length) {
      const auto [step_from, path] = search.front();
      search.pop();

      for (const auto& [step_to, move] :
           std::views::zip(step_from.neighborsUpDownLeftRight(),
                           std::array{'^', 'v', '<', '>'})) {
        if (keypad[step_to] == ' ') continue;
        if (step_to == to) {
          if (path.length() > shortest_length) {
            beyond_shortest_length = true;
            break;
          }
          shortest_length = path.length();
          paths[{keypad[from], keypad[to]}].emplace_back(path + move + 'A');

        } else {
          search.emplace(step_to, path + move);
        }
      }
    }
  }
  return paths;
}

[[nodiscard]] auto solutionsFor(const Paths& paths, const std::string& code)
    -> std::vector<std::string> {
  const auto path_for = [&](auto from, auto to) {
    return paths.at({from, to});
  };

  const auto to_string = [](auto vector) {
    return vector | std::views::join | std::ranges::to<std::string>();
  };

  return Utils::cartesian_product(
             std::views::zip("A" + code, code)                  //
             | std::views::transform(Utils::uncurry(path_for))  //
             | std::ranges::to<std::vector>())                  //
         | std::views::transform(to_string)                     //
         | std::ranges::to<std::vector>();
}

[[nodiscard]] auto calculateLength(const Paths& paths, const auto& lengths,
                                   const std::string& moves,
                                   size_t depth) -> size_t {
  static auto cache    = std::unordered_map<std::string, size_t>{};
  const auto cache_key = fmt::format("{:02}{}", depth, moves);
  if (cache.contains(cache_key)) return cache.at(cache_key);

  const auto step_length = [&](auto from, auto to) {
    return lengths.at({from, to});
  };
  const auto path_at = [&](auto from, auto to) { return paths.at({from, to}); };

  const auto calculate_next = [&](const auto& next_moves) {
    return calculateLength(paths, lengths, next_moves, depth - 1);
  };

  if (depth == 1) {
    return Utils::sum(std::views::zip("A" + moves, moves) |
                      std::views::transform(Utils::uncurry(step_length)));
  }

  auto length = size_t{};
  for (const auto& path : std::views::zip("A" + moves, moves) |
                              std::views::transform(Utils::uncurry(path_at))) {
    length += std::ranges::min(path | std::views::transform(calculate_next));
  }

  cache[cache_key] = length;
  return length;
}

[[nodiscard]] auto calculateComplexity(const std::vector<std::string>& codes,
                                       size_t depth) -> size_t {
  const auto number_pad_paths = pathsForKeypad({3, "789456123 0A"});
  const auto arrow_pad_paths  = pathsForKeypad({3, " ^A<v>"});

  const auto length_for_step = [&](auto step, const auto& solutions) {
    return std::make_pair(step, solutions.front().length());
  };

  const auto arrow_pad_lengths =
      arrow_pad_paths                                           //
      | std::views::transform(Utils::uncurry(length_for_step))  //
      | std::ranges::to<std::unordered_map>();

  const auto calculate_length = [&](const auto& solution) {
    return calculateLength(arrow_pad_paths, arrow_pad_lengths, solution, depth);
  };

  auto total = size_t{};
  for (const auto& code : codes) {
    const auto solutions = solutionsFor(number_pad_paths, code);
    const auto length =
        std::ranges::min(solutions | std::views::transform(calculate_length));
    total += Utils::from_chars<size_t>(code) * length;
  }

  return total;
}

}  // namespace Day21

TEST(Day_21_Keypad_Conundrum_SAMPLE) {
  const auto codes = Utils::readLines("21/sample.txt");
  EXPECT_EQ(Day21::calculateComplexity(codes, 2U), 126384);
  // No part 2 solution provided for the sample
}

//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/24
//

#include <bitset>
#include <filesystem>
#include <functional>  // IWYU pragma: keep

#include "external/ctre.hpp"
#include "testrunner/testrunner.h"
#include "utils/charconv.hh"
#include "utils/read_file.hh"

template <class... Ts>
struct overloads : Ts... {
  using Ts::operator()...;
};

namespace Day24 {

struct AND {
  [[nodiscard]] constexpr auto operator()(bool lhs, bool rhs) const -> bool {
    return lhs and rhs;
  }
};

struct OR {
  [[nodiscard]] constexpr auto operator()(bool lhs, bool rhs) const -> bool {
    return lhs or rhs;
  }
};

struct XOR {
  [[nodiscard]] constexpr auto operator()(bool lhs, bool rhs) const -> bool {
    return lhs xor rhs;
  }
};

using OP = std::variant<AND, OR, XOR>;

struct Rule {
  std::string in1{};
  std::string in2{};
  OP op{};
  std::string out{};
};

struct Device {
  std::unordered_map<std::string, bool> wire{};
  std::vector<Rule> rules{};
  std::bitset<64> x{};
  std::bitset<64> y{};
};

[[nodiscard]] auto readRules(const std::filesystem::path& path) -> Device {
  using namespace ctre::literals;  // NOLINT
  const auto file = Utils::readFile(path);

  auto device = Device{};
  for (auto [matched, wire, initial_value] :
       ctre::search_all<R"(([[:alnum:]]+): (\d))">(file)) {
    if (matched) {
      const auto wire_name   = wire.to_string();
      const auto value       = initial_value.to_number() != 0;
      device.wire[wire_name] = value;
      if (value and wire_name.starts_with('x'))
        device.x.set(Utils::from_chars<size_t>(wire_name.substr(1)));
      if (value and wire_name.starts_with('y'))
        device.y.set(Utils::from_chars<size_t>(wire_name.substr(1)));
    }
  }

  for (auto [matched, in1, op, in2, out] : ctre::search_all<
           R"(([[:alnum:]]+) ([[:alpha:]]+) ([[:alnum:]]+) -> ([[:alnum:]]+))">(
           file)) {
    if (matched) {
      auto rule = Rule{
          .in1 = in1.to_string(),
          .in2 = in2.to_string(),
          .op  = XOR{},
          .out = out.to_string(),
      };
      if (op.to_string() == "AND") rule.op = AND{};
      if (op.to_string() == "OR") rule.op = OR{};
      device.rules.emplace_back(rule);
    }
  }
  return device;
}

[[nodiscard]] auto followTheWires(Device device) -> size_t {
  while (!device.rules.empty()) {
    auto before = device.rules.size();
    for (auto rule = device.rules.begin(); rule != device.rules.end(); ++rule) {
      if (device.wire.contains(rule->in1) and device.wire.contains(rule->in2)) {
        device.wire[rule->out] = std::visit(
            overloads([&](auto& op) {
              return op(device.wire.at(rule->in1), device.wire.at(rule->in2));
            }),
            rule->op);
        device.rules.erase(rule);
        break;
      }
    }
    if (device.rules.size() == before) return 0;
  }

  auto result = std::bitset<64>{};
  for (const auto& [name, value] : device.wire) {
    if (!value or !name.starts_with('z')) continue;
    result.set(Utils::from_chars<size_t>(name.substr(1)));
  }

  return result.to_ullong();
}

}  // namespace Day24

TEST(Day_24_Crossed_Wires_SAMPLE) {
  const auto device = Day24::readRules("24/sample.txt");
  EXPECT_EQ(Day24::followTheWires(device), 2024);
  // TODO(AE): Part 2 star achieved by hand ;)
}

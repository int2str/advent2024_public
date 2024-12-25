//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/23
//

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <bitset>
#include <filesystem>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

#include "external/ctre.hpp"
#include "testrunner/testrunner.h"
#include "utils/nm_view.hh"
#include "utils/read_file.hh"

namespace Day23 {

using Hostname    = std::string;
using NetworkID   = std::string;
using Bits        = std::bitset<1024>;
using BitMap      = std::unordered_map<Hostname, Bits>;
using Netmask     = std::unordered_map<Hostname, Bits>;
using NetworkMap  = std::unordered_map<Hostname, std::unordered_set<Hostname>>;
using Connections = std::vector<std::pair<Hostname, Hostname>>;

struct Network {
  NetworkMap map;
  BitMap bits;
  Netmask mask;
};

[[nodiscard]] auto bitsFor(const NetworkMap& map) -> BitMap {
  auto bits = BitMap{};
  auto bit  = size_t{};
  for (const auto& [host, _] : map) bits[host].set(bit++);
  return bits;
}

[[nodiscard]] auto maskFor(const NetworkMap& map,
                           const BitMap& bits) -> Netmask {
  auto net_mask = Netmask{};
  for (const auto& [host, peers] : map) {
    net_mask[host] |= bits.at(host);
    for (const auto& computer : peers) net_mask[host] |= bits.at(computer);
  }
  return net_mask;
};

[[nodiscard]] auto makeNetwork(const std::filesystem::path& path) -> Network {
  using namespace ctre::literals;  // NOLINT
  auto map        = NetworkMap{};
  const auto file = Utils::readFile(path);
  for (auto [matched, c1, c2] :
       ctre::search_all<R"(([a-z]+)-([a-z]+)\s*)">(file)) {
    map[c1.to_string()].insert(c2.to_string());
    map[c2.to_string()].insert(c1.to_string());
  }
  const auto bits = bitsFor(map);
  return {.map = map, .bits = bits, .mask = maskFor(map, bits)};
}

[[nodiscard]] auto netID(std::vector<Hostname> hosts) -> NetworkID {
  std::sort(std::begin(hosts), std::end(hosts));
  return fmt::format("{}", fmt::join(hosts, ","));
}

[[nodiscard]] auto bestConnected(const Network& network) -> NetworkID {
  for (const auto& [host1, mask1] : network.mask) {
    auto bits_matched  = size_t{};
    auto hosts_matched = std::vector<Hostname>{host1};

    for (const auto& [host2, mask2] : network.mask) {
      if (host1 == host2) continue;

      const auto bits_in_common = mask1 & mask2;
      if (bits_in_common.count() > bits_matched) {
        bits_matched  = bits_in_common.count();
        hosts_matched = {host1};
      }
      if (bits_in_common.count() == bits_matched)
        hosts_matched.emplace_back(host2);
    }

    if (hosts_matched.size() == network.map.at(host1).size())
      return netID(hosts_matched);
  }

  return "???";
}

[[nodiscard]] auto t_Nodes(const Network& network) -> size_t {
  auto networks = std::unordered_set<NetworkID>{};
  for (const auto& [host, peers] : network.map) {
    if (!host.starts_with("t")) continue;
    for (const auto [peer1, peer2] : Utils::nm_const_view(peers)) {
      const auto compare = network.bits.at(host) | network.bits.at(*peer1) |
                           network.bits.at(*peer2);
      const auto mask = network.mask.at(host) & network.mask.at(*peer1) &
                        network.mask.at(*peer2) & compare;
      if (mask == compare) networks.insert(netID({host, *peer1, *peer2}));
    }
  }
  return networks.size();
}

}  // namespace Day23

TEST(Day_23_LAN_Party_SAMPLE) {
  const auto network = Day23::makeNetwork("23/sample.txt");
  EXPECT_EQ(Day23::t_Nodes(network), 7);
  EXPECT_EQ(Day23::bestConnected(network), "co,de,ka,ta");
}

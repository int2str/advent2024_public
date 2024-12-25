#ifndef UTILS_DIJKSTRAS_HH
#define UTILS_DIJKSTRAS_HH

#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace Utils::Detail {

template <typename KEY, typename VALUE>
struct default_map : std::unordered_map<KEY, VALUE> {
  static inline VALUE max_ = std::numeric_limits<VALUE>::max();
  [[nodiscard]] constexpr auto at_or_max(const KEY& key) const -> const VALUE& {
    if (this->contains(key)) return this->at(key);
    return max_;
  }
};

}  // namespace Utils::Detail

namespace Utils {

template <typename DISTANCE, typename EDGE>
struct WeightedEdge {
  DISTANCE distance;
  EDGE edge;

  [[nodiscard]] constexpr auto operator<(const WeightedEdge& other) const
      -> bool {
    // NOTE(AE): REVERSE ORDERING - MIN ELEMENT FIRST
    return other.distance < distance;
  }
};

template <typename DISTANCE, typename EDGE>
[[nodiscard]] auto dijkstra(WeightedEdge<DISTANCE, EDGE> start,
                            auto&& adjacent) {
  auto distances = Detail::default_map<EDGE, DISTANCE>{};
  auto previous  = std::unordered_map<EDGE, std::unordered_set<EDGE>>{};

  auto queue = std::priority_queue<WeightedEdge<DISTANCE, EDGE>>{};
  queue.push(start);

  while (!queue.empty()) {
    const auto [distance, current] = queue.top();
    queue.pop();

    for (const auto [distance_to, other] : adjacent(current)) {
      if (distance + distance_to < distances.at_or_max(other)) {
        distances[other] = distance + distance_to;
        queue.push({distances[other], other});
      }
      if (distance + distance_to <= distances.at_or_max(other))
        previous[other].insert(current);
    }
  }

  return std::make_pair(distances, previous);
}

template <typename DISTANCE, typename EDGE>
[[nodiscard]] auto dijkstra(WeightedEdge<DISTANCE, EDGE> start, EDGE finish,
                            auto&& adjacent) {
  auto distances = Detail::default_map<EDGE, DISTANCE>{};

  auto queue = std::priority_queue<WeightedEdge<DISTANCE, EDGE>>{};
  queue.push(start);

  while (!queue.empty()) {
    const auto [distance, current] = queue.top();
    queue.pop();

    if (current == finish) return distance;

    for (const auto [distance_to, other] : adjacent(current)) {
      if (distance + distance_to < distances.at_or_max(other)) {
        distances[other] = distance + distance_to;
        queue.push({distances[other], other});
      }
    }
  }

  return DISTANCE{};
}

}  // namespace Utils

#endif  // UTILS_DIJKSTRAS_HH

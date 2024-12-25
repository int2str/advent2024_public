#ifndef COORDINATE_MAP_HH
#define COORDINATE_MAP_HH

#include <unordered_map>

#include "coordinate.hh"

namespace Utils {

struct CoordinateHash {
  [[nodiscard]] constexpr auto operator()(const Coordinate& coord) const
      -> size_t {
    return static_cast<size_t>(std::hash<int>()(coord.x) ^
                               std::hash<int>()(coord.y));
  }
};

template <typename T>
using CoordinateMap = std::unordered_map<Coordinate, T, CoordinateHash>;

}  // namespace Utils

namespace std {

template <>
struct hash<Utils::Coordinate> {
  [[nodiscard]] auto operator()(
      const Utils::Coordinate& coordinate) const noexcept -> size_t {
    return Utils::CoordinateHash()(coordinate);
  }
};

}  // namespace std

#endif  // COORDINATE_MAP_HH

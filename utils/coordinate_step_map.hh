#ifndef COORDINATE_STEP_MAP_HH
#define COORDINATE_STEP_MAP_HH

#include <unordered_map>

#include "coordinate_map.hh"
#include "coordinate_step.hh"

namespace Utils {

struct StepHash {
  [[nodiscard]] constexpr auto operator()(const Step& step) const -> size_t {
    return static_cast<size_t>(Utils::CoordinateHash()(step.position) ^
                               Utils::CoordinateHash()(step.direction));
  }
};

template <typename T>
using StepMap = std::unordered_map<Step, T, StepHash>;

}  // namespace Utils

namespace std {

template <>
struct hash<Utils::Step> {
  [[nodiscard]] auto operator()(const Utils::Step& step) const noexcept
      -> size_t {
    return Utils::StepHash()(step);
  }
};

}  // namespace std

#endif  // COORDINATE_STEP_MAP_HH

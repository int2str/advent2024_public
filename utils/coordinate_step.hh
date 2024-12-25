#ifndef COORDINATE_STEP_HH
#define COORDINATE_STEP_HH

#include "coordinate.hh"

namespace Utils {

struct Step {
  Coordinate position;
  Coordinate direction;

  [[nodiscard]] constexpr auto operator<=>(const Step&) const = default;

  [[nodiscard]] constexpr auto next() const -> Coordinate {
    return position + direction;
  }
};

}  // namespace Utils

#endif  // COORDINATE_STEP_HH

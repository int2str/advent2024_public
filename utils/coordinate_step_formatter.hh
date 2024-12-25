#ifndef COORDINATE_STEP_FORMATTER_HH
#define COORDINATE_STEP_FORMATTER_HH

#include <fmt/format.h>

#include "coordinate_formatter.hh"  // IWYU pragma: keep
#include "coordinate_step.hh"

template <>
struct fmt::formatter<Utils::Step> : fmt::formatter<Utils::Coordinate> {
  template <typename FormatContext>
  auto format(const Utils::Step& step, FormatContext& ctx) const {
    fmt::formatter<Utils::Coordinate>::format(step.position, ctx);
    return fmt::format_to(ctx.out(), "->{}", step.direction);
  }
};

#endif  // COORDINATE_STEP_FORMATTER_HH

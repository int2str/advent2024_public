#ifndef COORDINATE_FORMATTER_HH
#define COORDINATE_FORMATTER_HH

#include <fmt/format.h>

#include "coordinate.hh"

template <typename INTEGER_TYPE>
struct fmt::formatter<Utils::CoordinateBase<INTEGER_TYPE>> {
  enum class View : uint8_t { Raw_XY, Editor_YX };

  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    auto it = ctx.begin();
    while (it != ctx.end() and *it != '}') {
      if (*it == 'e') {
        view_ = View::Editor_YX;
        base_ = 1;

      } else if (*it == 'r') {
        view_ = View::Raw_XY;
        base_ = 0;

      } else {
        if !consteval {
          throw fmt::format_error(
              fmt::format("Invalid Coordinate format specifier '{}'", *it));
        }
        break;
      }
      ++it;
    }
    return it;
  }

  template <typename FormatContext>
  auto format(const Utils::CoordinateBase<INTEGER_TYPE>& coordinate,
              FormatContext& ctx) const {
    switch (view_) {
      case View::Editor_YX:
        return fmt::format_to(ctx.out(), "{}:{}", coordinate.y + base_,
                              coordinate.x + base_);
      case View::Raw_XY:
        return fmt::format_to(ctx.out(), "{}/{}", coordinate.x + base_,
                              coordinate.y + base_);
      default:
        throw fmt::format_error("Invalid coordinate format");
    }
    return ctx.out();
  }

 private:
  View view_{View::Raw_XY};
  int base_{0};
};

#endif  // COORDINATE_FORMATTER_HH

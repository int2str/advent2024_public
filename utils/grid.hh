#ifndef UTILS_GRID_HH
#define UTILS_GRID_HH

// Original by Sy Brand
// --> https://github.com/TartanLlama/aoc-2024/blob/main/src/grid.hpp

#include <cmath>
#include <istream>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "coordinate.hh"

namespace Utils::OutOfBoundsPolicy {

template <auto DEFAULT_VALUE>
struct Default {
  static constexpr auto check_bounds = true;
  static inline auto default_value   = DEFAULT_VALUE;
  static constexpr auto outOfBounds() -> decltype(default_value)& {
    return default_value;
  }
};

struct Undefined {
  static constexpr auto check_bounds = false;
};

struct Throw {
  static constexpr auto check_bounds = true;
  static void outOfBounds() {
    // TODO(AE): Error message should show coordinate
    throw std::out_of_range("Out of bounds grid access");
  }
};

};  // namespace Utils::OutOfBoundsPolicy

namespace Utils::CharConverter {

template <typename T>
struct fromAscii {
  [[nodiscard]] constexpr auto operator()(char chr) -> T {
    return static_cast<T>(chr - '0');
  }
};

}  // namespace Utils::CharConverter

namespace Utils {

template <typename STORE_AS, typename OOB_POLICY = OutOfBoundsPolicy::Undefined>
class Grid {
  size_t width_{};
  size_t height_{};
  std::vector<STORE_AS> data_{};

 public:
  // Convenience

  static auto from(std::istream& input) -> Grid {
    auto filter_newlines = [](auto chr) { return chr != '\n'; };
    auto chars           = std::string{};
    std::getline(input, chars, '\0');
    const auto width = chars.find('\n');
    return Grid{width, chars | std::views::filter(filter_newlines)};
  }

  // Constructors

  Grid(size_t width, size_t height)
      : width_{width}, height_{height}, data_(width * height) {}

  template <typename CHARACTER_RANGE>
    requires std::is_same_v<STORE_AS, char>
  Grid(size_t width, CHARACTER_RANGE&& input_range)
      : width_{width}, data_{std::begin(input_range), std::end(input_range)} {
    height_ = data_.size() / width_;
  }

  template <typename CHARACTER_RANGE,
            typename CONVERTER = CharConverter::fromAscii<STORE_AS>>
  Grid(size_t width, CHARACTER_RANGE&& input_range, CONVERTER&& convert)
      : width_{width} {
    const auto distance = std::ranges::distance(input_range);
    height_             = distance / width_;
    for (auto&& element : input_range) data_.push_back(convert(element));
  }

  // Data access

  [[nodiscard]] constexpr auto operator[](size_t x, size_t y) -> STORE_AS& {
    if constexpr (OOB_POLICY::check_bounds) {
      if (!inBounds(Coordinate{static_cast<int>(x), static_cast<int>(y)}))
        return OOB_POLICY::outOfBounds();
    }
    return data_[y * width_ + x];
  }

  [[nodiscard]] constexpr auto operator[](size_t x,
                                          size_t y) const -> const STORE_AS& {
    if constexpr (OOB_POLICY::check_bounds) {
      if (!inBounds(Coordinate{static_cast<int>(x), static_cast<int>(y)}))
        return OOB_POLICY::outOfBounds();
    }
    return data_[y * width_ + x];
  }

  [[nodiscard]] constexpr auto operator[](Coordinate coordinate) -> STORE_AS& {
    if constexpr (OOB_POLICY::check_bounds) {
      if (!inBounds(coordinate)) return OOB_POLICY::outOfBounds();
    }
    return data_[static_cast<size_t>(coordinate.y) * width_ +
                 static_cast<size_t>(coordinate.x)];
  }

  [[nodiscard]] constexpr auto operator[](Coordinate coordinate) const
      -> const STORE_AS& {
    if constexpr (OOB_POLICY::check_bounds) {
      if (!inBounds(coordinate)) return OOB_POLICY::outOfBounds();
    }
    return data_[static_cast<size_t>(coordinate.y) * width_ +
                 static_cast<size_t>(coordinate.x)];
  }

  // Utility

  [[nodiscard]] constexpr auto height() const { return height_; };

  [[nodiscard]] constexpr auto width() const { return width_; };

  [[nodiscard]] constexpr auto inBounds(Coordinate coordinate) const -> bool {
    return coordinate.x >= 0 and static_cast<size_t>(coordinate.x) < width_ and
           coordinate.y >= 0 and static_cast<size_t>(coordinate.y) < height_;
  }

  void clear() { std::fill(data_.begin(), data_.end(), STORE_AS{}); }

  // Coordinate Generators

  [[nodiscard]] auto coordinates() const {
    return std::views::cartesian_product(std::views::iota(0U, height_),
                                         std::views::iota(0U, width_))  //
           | std::views::transform([](auto&& xy_tuple) {
               return Coordinate{static_cast<int>(std::get<1>(xy_tuple)),
                                 static_cast<int>(std::get<0>(xy_tuple))};
             });
  }

  // Algorithms

  [[nodiscard]] auto find(const STORE_AS& what) const
      -> std::optional<Coordinate> {
    for (const auto search : coordinates())
      if ((*this)[search] == what) return search;
    return std::nullopt;
  }
};

}  // namespace Utils

#endif  // UTILS_GRID_HH

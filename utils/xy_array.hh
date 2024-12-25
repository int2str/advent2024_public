#ifndef XYARRAY_HH
#define XYARRAY_HH

#include <vector>

#include "coordinate.hh"

namespace Utils {

template <typename T, std::size_t WIDTH, std::size_t HEIGHT>
class xy_array {
 public:
  xy_array() { data_.resize(WIDTH * HEIGHT); }

  [[nodiscard]] auto operator[](const Coordinate& coordinate) -> T& {
    return data_.at(idxFor(static_cast<std::size_t>(coordinate.x),
                           static_cast<std::size_t>(coordinate.y)));
  }

  [[nodiscard]] auto operator[](const Coordinate& coordinate) const
      -> const T& {
    return data_.at(idxFor(static_cast<std::size_t>(coordinate.x),
                           static_cast<std::size_t>(coordinate.y)));
  }

  [[nodiscard]] auto operator[](std::size_t x, std::size_t y) -> T& {
    return data_.at(idxFor(x, y));
  }

  [[nodiscard]] auto operator[](std::size_t x,
                                std::size_t y) const -> const T& {
    return data_.at(idxFor(x, y));
  }

 private:
  std::vector<T> data_{};

  [[nodiscard]] static constexpr auto idxFor(std::size_t x,
                                             std::size_t y) -> std::size_t {
    return (y * WIDTH) + x;
  }
};

}  // namespace Utils

#endif  // XYARRAY_HH

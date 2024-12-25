#ifndef UTILS_SUM_HH
#define UTILS_SUM_HH

#include <algorithm>
#include <ranges>

namespace Utils {

template <typename RANGE>
[[nodiscard]] auto sum(RANGE&& range) {
  return std::ranges::fold_left(range, std::ranges::range_value_t<RANGE>{},
                                std::plus{});
}

}  // namespace Utils

#endif  // UTILS_SUM_HH

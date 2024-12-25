#ifndef UTILS_VECTOR_CARTESIAN_PRODUCT_HH
#define UTILS_VECTOR_CARTESIAN_PRODUCT_HH

#include <vector>

namespace Utils {

template <typename T>
[[nodiscard]] auto cartesian_product(const std::vector<std::vector<T>>& in)
    -> std::vector<std::vector<T>> {
  auto results = std::vector<std::vector<T>>{{}};

  for (const auto& new_values : in) {
    auto next_results = std::vector<std::vector<T>>{};

    for (const auto& result : results) {
      for (const auto& value : new_values) {
        next_results.emplace_back(result);
        next_results.back().emplace_back(value);
      }
    }

    results = std::move(next_results);
  }

  return results;
}

}  // namespace Utils

#endif  // UTILS_VECTOR_CARTESIAN_PRODUCT_HH

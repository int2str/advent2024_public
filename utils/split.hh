#ifndef SPLIT_HH
#define SPLIT_HH

#include <array>
#include <charconv>
#include <string>
#include <string_view>
#include <vector>

namespace Utils {

template <typename T, size_t SIZE>
[[nodiscard]] auto split(std::string_view str,
                         std::string_view delimiter) -> std::array<T, SIZE> {
  auto values = std::array<T, SIZE>{};
  auto idx    = size_t{};
  while (idx != SIZE and !str.empty()) {
    const auto delimiter_at = str.find(delimiter);
    if (delimiter_at != std::string_view::npos) {
      std::from_chars(str.begin(), str.begin() + delimiter_at, values[idx]);
      str.remove_prefix(delimiter_at + delimiter.length());
    } else {
      std::from_chars(str.begin(), str.end(), values[idx]);
      str = {};
    }
    ++idx;
  }
  return values;
}

template <typename T>
[[nodiscard]] auto split(std::string_view str,
                         std::string_view delimiter) -> std::vector<T> {
  auto values = std::vector<T>{};
  while (!str.empty()) {
    const auto delimiter_at = str.find(delimiter);
    auto value              = T{};
    if (delimiter_at != std::string_view::npos) {
      std::from_chars(str.begin(), str.begin() + delimiter_at, value);
      str.remove_prefix(delimiter_at + delimiter.length());
    } else {
      std::from_chars(str.begin(), str.end(), value);
      str = {};
    }
    values.emplace_back(value);
  }
  return values;
}

[[nodiscard]] constexpr auto str_split(std::string_view str,
                                       std::string_view delimiter)
    -> std::vector<std::string> {
  auto values = std::vector<std::string>{};
  while (!str.empty()) {
    const auto delimiter_at = str.find(delimiter);
    auto value              = std::string{};
    if (delimiter_at != std::string_view::npos) {
      values.emplace_back(str.begin(), delimiter_at);
      str.remove_prefix(delimiter_at + delimiter.length());
    } else {
      values.emplace_back(str.begin(), str.end());
      str = {};
    }
  }
  return values;
}

}  // namespace Utils

#endif  // SPLIT_HH

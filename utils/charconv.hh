#ifndef UTILS_CHARCONV_HH
#define UTILS_CHARCONV_HH

#include <charconv>
#include <string_view>

namespace Utils {

template <typename T>
[[nodiscard]] constexpr auto from_chars(std::string_view chars) -> T {
  auto value = T{};
  std::from_chars(std::begin(chars), std::end(chars), value);
  return value;
}

}  // namespace Utils

#endif  // UTILS_CHARCONV_HH

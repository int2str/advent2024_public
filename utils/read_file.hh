#ifndef READ_FILE_HH
#define READ_FILE_HH

#include <filesystem>
#include <ranges>
#include <string>
#include <vector>

namespace Utils {

[[nodiscard]] auto readFile(const std::filesystem::path& path)
    -> std::vector<char>;

[[nodiscard]] auto readLines(const std::filesystem::path& path)
    -> std::vector<std::string>;

template <typename TRANSFORMER>
auto readFileXY(const std::filesystem::path& path, TRANSFORMER&& transformer) {
  const auto lines = readLines(path) | std::views::enumerate;
  for (const auto& [y, line] : lines) {
    for (const auto& [x, chr] : line | std::views::enumerate)
      transformer(static_cast<size_t>(x), static_cast<size_t>(y), chr);
  }
  return transformer;
}

}  // namespace Utils

#endif  // READ_FILE_HH

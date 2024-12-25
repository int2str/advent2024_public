#include "read_file.hh"

#include <filesystem>
#include <fstream>
#include <vector>

namespace Utils {

auto readFile(const std::filesystem::path& path) -> std::vector<char> {
  auto file_stream = std::ifstream(std::string{path}, std::ios_base::binary);
  if (!file_stream.good()) return {};

  file_stream.seekg(0, std::ios_base::end);
  const auto file_size = static_cast<size_t>(file_stream.tellg());

  auto contents = std::vector<char>(file_size);
  file_stream.seekg(0);
  file_stream.read(contents.data(), static_cast<std::streamsize>(file_size));
  return contents;
}

auto readLines(const std::filesystem::path& path) -> std::vector<std::string> {
  auto file_stream = std::ifstream(std::string{path}, std::ios_base::binary);
  if (!file_stream.good()) return {};

  std::string line;
  auto lines = std::vector<std::string>{};
  while (std::getline(file_stream, line)) lines.emplace_back(line);

  return lines;
}

}  // namespace Utils

#ifndef COORDINATE_SET_HH
#define COORDINATE_SET_HH

#include <bitset>
#include <cstdint>
#include <limits>
#include <ranges>

#include "coordinate.hh"

namespace Utils {

class CoordinateSet {
  using Bits = std::bitset<std::numeric_limits<uint16_t>::max()>;

  Bits bits_{};

  [[nodiscard]] static constexpr auto idxFor(const Coordinate& coordinate)
      -> uint16_t {
    return static_cast<uint16_t>(
        (static_cast<uint16_t>(coordinate.y) << 8) +    // NOLINT
        (static_cast<uint16_t>(coordinate.x) & 0xFF));  // NOLINT
  }

 public:
  class Iterator {
    const Bits* bits_p_{nullptr};
    size_t idx_{};

    friend CoordinateSet;
    constexpr explicit Iterator(const Bits* bits, size_t idx)
        : bits_p_{bits}, idx_{idx} {
      if (idx_ < bits_p_->size() and !bits_p_->test(idx)) advance();
    }

    void advance() {
      do {
        ++idx_;
      } while (idx_ < bits_p_->size() and !bits_p_->test(idx_));
    }

   public:
    using difference_type = int32_t;
    using value_type      = Coordinate;

    Iterator() = default;

    [[nodiscard]] constexpr auto operator*() const -> Coordinate {
      return {.x = static_cast<int>(idx_ & 0xFF),  // NOLINT
              .y = static_cast<int>(idx_ >> 8)};   // NOLINT
    }

    constexpr auto operator++() -> Iterator& {
      advance();
      return *this;
    }

    constexpr auto operator++(int) -> Iterator {
      const auto pre = *this;
      advance();
      return pre;
    }

    [[nodiscard]] constexpr auto operator<=>(const Iterator&) const = default;
  };

  using const_iterator = Iterator;
  using iterator       = Iterator;
  using value_type     = Coordinate;

  CoordinateSet() = default;

  explicit CoordinateSet(Coordinate coordinate) { insert(coordinate); }

  template <typename RANGE>
  CoordinateSet(std::from_range_t /*unused*/, RANGE&& range) {
    for (auto coordinate : range) insert(coordinate);
  }

  constexpr void insert(const Coordinate& coordinate) {
    bits_.set(idxFor(coordinate));
  }

  constexpr void erase(const Coordinate& coordinate) {
    bits_.reset(idxFor(coordinate));
  }

  constexpr void clear() { bits_ = 0; }

  [[nodiscard]] constexpr auto contains(const Coordinate& coordinate) const
      -> bool {
    const auto idx = idxFor(coordinate);
    return idx < bits_.size() and bits_.test(idx);
  }

  [[nodiscard]] constexpr auto operator[](const Coordinate& coordinate) const
      -> bool {
    return contains(coordinate);
  }

  [[nodiscard]] constexpr auto size() const -> size_t { return bits_.size(); }

  [[nodiscard]] constexpr auto count() const -> size_t { return bits_.count(); }

  [[nodiscard]] constexpr auto empty() const -> bool { return bits_.none(); }

  [[nodiscard]] constexpr auto begin() const -> Iterator {
    return Iterator{&bits_, 0};
  }

  [[nodiscard]] constexpr auto end() const -> Iterator {
    return Iterator{&bits_, bits_.size()};
  }
};
}  // namespace Utils

#endif  // COORDINATE_SET_HH



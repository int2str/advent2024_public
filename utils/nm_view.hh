#ifndef NM_ITERATOR_HH
#define NM_ITERATOR_HH

#include <fmt/core.h>

#include <iterator>

namespace Utils {

template <typename RANGE>
class nm_const_view {
 public:
  struct Iterator {
    using iterator_t = typename RANGE::const_iterator;

    iterator_t n{};
    iterator_t m{};
    iterator_t last{};

    constexpr auto operator++() -> Iterator& {
      if (m != last and ++m == last) {
        ++n;
        m = std::next(n);
        if (m == last) *this = {};
      }
      return *this;
    }

    [[nodiscard]] constexpr auto operator<=>(const Iterator&) const = default;

    [[nodiscard]] constexpr auto operator*() const
        -> std::pair<iterator_t, iterator_t> {
      return {n, m};
    }

    [[nodiscard]] constexpr auto valid() const -> bool {
      return n != last and m != last;
    }
  };

  explicit nm_const_view(const RANGE& range)
      : it_{range.begin(), std::next(range.begin()), range.end()} {}

  [[nodiscard]] constexpr auto begin() -> Iterator {
    return it_.valid() ? it_ : Iterator{};
  }

  [[nodiscard]] constexpr auto end() -> Iterator { return {}; }

 private:
  Iterator it_;
};

// TODO(AE): Handle constness automatically...
template <typename RANGE>
class nm_view {
 public:
  struct Iterator {
    using iterator_t = typename RANGE::iterator;

    iterator_t n{};
    iterator_t m{};
    iterator_t last{};

    constexpr auto operator++() -> Iterator& {
      if (m != last and ++m == last) {
        ++n;
        m = std::next(n);
        if (m == last) *this = {};
      }
      return *this;
    }

    [[nodiscard]] constexpr auto operator<=>(const Iterator&) const = default;

    [[nodiscard]] constexpr auto operator*()
        -> std::pair<iterator_t, iterator_t> {
      return {n, m};
    }

    [[nodiscard]] constexpr auto valid() const -> bool {
      return n != last and m != last;
    }
  };

  explicit nm_view(RANGE& range)
      : it_{range.begin(), std::next(range.begin()), range.end()} {}

  [[nodiscard]] constexpr auto begin() -> Iterator {
    return it_.valid() ? it_ : Iterator{};
  }

  [[nodiscard]] constexpr auto end() -> Iterator { return {}; }

 private:
  Iterator it_;
};

}  // namespace Utils

#endif  // NM_ITERATOR_HH

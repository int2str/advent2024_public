//
// nm_pairs_view
//
// A simple range iterator providing a "bubble" view over a range,
// returning n*m pairs.
//
// Copyright (c) 2024 Andre Eisenbch - @int2str.net
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#ifndef NM_PAIRS_VIEW_HH
#define NM_PAIRS_VIEW_HH

#include <ranges>
#include <utility>

namespace Utils {

template <std::ranges::forward_range RANGE>
struct nm_sentinel {};

template <std::ranges::forward_range RANGE>
class nm_iterator {
  using base_iterator   = std::ranges::iterator_t<RANGE>;
  using base_sentinel   = std::ranges::sentinel_t<RANGE>;
  using base_value_type = std::ranges::range_value_t<RANGE>;
  using base_reference  = std::ranges::range_reference_t<RANGE>;

  base_iterator n_{};
  base_iterator m_{};
  base_sentinel end_{};

 public:
  using value_type        = std::pair<base_value_type, base_value_type>;
  using reference         = std::pair<base_reference, base_reference>;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  [[nodiscard]] constexpr nm_iterator() = default;

  [[nodiscard]] constexpr nm_iterator(base_iterator begin, base_sentinel end)
      : n_{begin}, m_{std::next(n_)}, end_{end} {}

  constexpr auto operator++() -> nm_iterator& {
    if (++m_ == end_) m_ = std::next(++n_);
    return *this;
  }

  [[nodiscard]] constexpr auto operator++(int) -> nm_iterator {
    const auto pre = *this;
    ++(*this);
    return pre;
  }

  [[nodiscard]] constexpr auto operator*() const -> reference {
    return {*n_, *m_};
  }

  [[nodiscard]] constexpr auto operator<=>(const nm_iterator&) const = default;

  [[nodiscard]] constexpr auto operator==(
      const nm_sentinel<RANGE>& /*unused*/) const -> bool {
    return n_ == end_ or m_ == end_;
  }
};

template <std::ranges::forward_range RANGE>
  requires std::ranges::view<RANGE>
class nm_pairs_view : public std::ranges::view_interface<nm_pairs_view<RANGE>> {
  RANGE base_;

 public:
  [[nodiscard]] constexpr nm_pairs_view() = default;

  [[nodiscard]] constexpr explicit nm_pairs_view(RANGE range)
      : base_{std::move(range)} {}

  [[nodiscard]] constexpr auto begin() const -> nm_iterator<RANGE> {
    return nm_iterator<RANGE>{std::ranges::begin(base_),
                              std::ranges::end(base_)};
  }

  [[nodiscard]] constexpr auto end() const -> nm_sentinel<RANGE> { return {}; }
};

template <std::ranges::sized_range RANGE>
nm_pairs_view(RANGE&&) -> nm_pairs_view<std::views::all_t<RANGE>>;

}  // namespace Utils

namespace Utils::views {

struct nm_pairs_fn : std::ranges::range_adaptor_closure<nm_pairs_fn> {
  template <typename RANGE>
  constexpr auto operator()(RANGE&& range) const {
    return nm_pairs_view{std::forward<RANGE>(range)};
  }
};

constexpr inline auto nm_pairs = nm_pairs_fn{};

}  // namespace Utils::views

#endif  // NM_PAIRS_VIEW_HH

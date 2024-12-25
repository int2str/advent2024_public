#ifndef ONE_OF_HH
#define ONE_OF_HH

#include <tuple>

template <typename... Ts>
struct one_of : private std::tuple<Ts...> {
  using std::tuple<Ts...>::tuple;

  template <typename T>
  [[nodiscard]] constexpr auto operator==(const T& val) const noexcept -> bool {
    const auto& tup = static_cast<const std::tuple<Ts...>&>(*this);
    return std::apply(
        [&val](const auto&... args) { return ((args == val) || ...); }, tup);
  }
};

template <typename... Ts>
one_of(Ts...) -> one_of<Ts...>;

#endif  // ONE_OF_HH

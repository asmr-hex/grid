#ifndef UTIL_MATCH_H
#define UTIL_MATCH_H

#include <utility>
#include <variant>


namespace detail {
  template<typename...T>
  struct overloaded : T... {
    using T::operator()...;
  };

  template<typename...T>
  overloaded(T...) -> overloaded<T...>;
}

template<typename Variant, typename...Matcher>
auto match(Variant&& variant, Matcher&&... matcher) {
  return std::visit(
                    detail::overloaded{std::forward<Matcher>(matcher)...},
                    std::forward<Variant>(variant));
}

#endif

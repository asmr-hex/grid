#ifndef UTIL_MATCH_H
#define UTIL_MATCH_H

#include <utility>
#include <variant>


namespace Visitor {
  template<typename...Ts>
  struct from_lambdas : Ts... {
    // construct an overload set for the operator() function
    // by bringing into scope (via parameter pack expansion)
    // the operator() function for each inherited type.
    //
    // note: we are interested in operator() because the types
    // we are passing in are lambda functions, which under the
    // hood are closure classes which use the operator() to call
    // the lambda.
    using Ts::operator()...;
  };

  template<typename...Ts>
  from_lambdas(Ts...) -> from_lambdas<Ts...>;
}

template<typename Variant, typename...Matcher>
auto match(Variant&& variant, Matcher&&... matcher) {
  return std::visit(
                    // TODO (coco|12.18.2019) we might want to not keep constructing this Vistitor lambda over and over...
                    // might be good to create a Match class which constructs this visitor functor only once!
                    Visitor::from_lambdas{std::forward<Matcher>(matcher)...},
                    std::forward<Variant>(variant));
}

#endif

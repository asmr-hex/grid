#include <catch.hpp>

#include <variant>

#include "anemone/state/state.hpp"
#include "anemone/util/match.hpp"


SCENARIO( "a State can represent a single value, collection, or composition of values" ) {

  using namespace State;
  
  GIVEN( "a string State" ) {

    using action_t = std::variant<int, bool>;
    //bool noop = false;
    
    auto leaf = State<std::string>::with_reducer<action_t>
      ([] (std::string state, action_t action) -> std::string {
         return
           match(action,
                 [&] (int a) {
                   return state;
                 },
                 [&] (auto& a) {
                   return state;
                 });
       }, "hello");
  }
}

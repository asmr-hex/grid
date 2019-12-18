#include <catch.hpp>

#include <variant>

#include "anemone/state/state.hpp"
#include "anemone/util/match.hpp"

struct composite {
  std::string a;
  int b;
};

SCENARIO( "a State can represent a single value, collection, or composition of values" ) {

  using namespace State;
  
  GIVEN( "a string State" ) {

    using action_t = std::variant<int, bool>;
    //bool noop = false;
    
    auto leaf_1 = State<std::string>::with_reducer<action_t>
      ("hello",
       [] (std::string state, action_t action) -> std::string {
         return
           match(action,
                 [&] (int a) {
                   return state;
                 },
                 [&] (auto& a) {
                   return state;
                 });
       });
    
    auto leaf_2 = State<int>::with_reducer<action_t>
      (888,
       [] (int state, action_t action) -> int {
         return
           match(action,
                 [&] (int a) {
                   return state;
                 },
                 [&] (auto& a) {
                   return state;
                 });
       });

    auto leaf_3 = State<int>::with_reducer<action_t>
      (666,
       [] (int state, action_t action) -> int {
         return
           match(action,
                 [&] (int a) {
                   return state;
                 },
                 [&] (auto& a) {
                   return state;
                 });
       });
    
    
    auto c1 = State<composite>
      ::with_actions<action_t>
      ::compose<std::string, int, int>
      ([] (std::string a, int b, int c) -> composite {
         return {
                 .a = a,
                 .b = b,
         };
       }, leaf_1, leaf_2, leaf_3);
  }
}

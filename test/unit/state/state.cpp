#include <catch.hpp>

#include <cctype>
#include <string>
#include <variant>
#include <algorithm>

#include "anemone/state/state.hpp"
#include "anemone/util/match.hpp"

struct composite {
  std::string a;
  int b;
};

namespace TestAction {
  struct change_str {
    std::string new_str;
  };

  struct lowercase_str {};
}


SCENARIO( "a State can represent a single value, collection, or composition of values" ) {

  using namespace State;
  
  GIVEN( "a string State and a change_str & lowercase_str action" ) {

    using action_t = std::variant<TestAction::change_str,
                                  TestAction::lowercase_str>;

    auto str_state = State<std::string>::with_reducer<action_t>
      ("HELLo",
       [] (std::string state, action_t action) -> std::string {
         return
           match(action,
                 [&] (const TestAction::change_str& a) {
                   return a.new_str;
                 },
                 [&] (const TestAction::lowercase_str&) {
                   std::transform(state.begin(), state.end(), state.begin(),
                                  [](unsigned char c) -> unsigned char { return std::tolower(c); });
                   return state;
                 });
       });

    WHEN( "we reduce with a change_str action" ) {
      REQUIRE ( str_state->value() == "HELLo" );
      str_state->reduce(TestAction::change_str{ .new_str = "hOly mOly" });
      
      THEN( "the state changes to the new string" ) {
        REQUIRE ( str_state->value() == "hOly mOly" );  
      }
    }
    
    // auto leaf_1 = State<std::string>::with_reducer<action_t>
    //   ("hello",
    //    [] (std::string state, action_t action) -> std::string {
    //      return
    //        match(action,
    //              [&] (int a) {
    //                return state;
    //              },
    //              [&] (auto& a) {
    //                return state;
    //              });
    //    });
    
    // auto leaf_2 = State<int>::with_reducer<action_t>
    //   (888,
    //    [] (int state, action_t action) -> int {
    //      return
    //        match(action,
    //              [&] (int a) {
    //                return state;
    //              },
    //              [&] (auto& a) {
    //                return state;
    //              });
    //    });

    // auto leaf_3 = State<int>::with_reducer<action_t>
    //   (666,
    //    [] (int state, action_t action) -> int {
    //      return
    //        match(action,
    //              [&] (int a) {
    //                return state;
    //              },
    //              [&] (auto& a) {
    //                return state;
    //              });
    //    });
    
    
    // auto c1 = State<composite>
    //   ::with_actions<action_t>
    //   ::compose<std::string, int, int>
    //   ([] (std::string a, int b, int c) -> composite {
    //      return {
    //              .a = a,
    //              .b = b,
    //      };
    //    }, leaf_1, leaf_2, leaf_3);
  }
}

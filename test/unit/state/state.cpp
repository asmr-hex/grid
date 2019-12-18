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
    
    auto leaf_1 = std::make_shared<state_t<std::string, action_t> >
      (State<std::string>::with_reducer<action_t>
       ([] (std::string state, action_t action) -> std::string {
          return
            match(action,
                  [&] (int a) {
                    return state;
                  },
                  [&] (auto& a) {
                    return state;
                  });
        }, "hello"));
    
    auto leaf_2 = std::make_shared<state_t<int, action_t> >
      (State<int>::with_reducer<action_t>
       ([] (int state, action_t action) -> int {
          return
            match(action,
                  [&] (int a) {
                    return state;
                  },
                  [&] (auto& a) {
                    return state;
                  });
        }, 888));

    auto leaf_3 = std::make_shared<state_t<int, action_t> >
      (State<int>::with_reducer<action_t>
       ([] (int state, action_t action) -> int {
          return
            match(action,
                  [&] (int a) {
                    return state;
                  },
                  [&] (auto& a) {
                    return state;
                  });
        }, 666));
    
    
    auto c1 = std::make_shared<state_t<composite, action_t> >
      (State<composite>
       ::with_actions<action_t>
       ::compose<std::string, int, int>
       ([] (std::string a, int b, int c) -> composite {
          return {
                  .a = a,
                  .b = b,
          };
        }, leaf_1, leaf_2, leaf_3));
  }
}

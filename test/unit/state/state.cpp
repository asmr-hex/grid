#include <catch.hpp>

#include <cctype>
#include <string>
#include <variant>
#include <algorithm>

#include "anemone/state/state.hpp"
#include "anemone/util/match.hpp"


namespace TestAction {
  struct change_str {
    std::string new_str;
  };
  struct lowercase_str {};
  struct increment{};
  struct change_state {
    std::string new_state;
  };
  struct append {
    int number = 666;
  };
  struct clear {};
}

namespace TestState {
  struct metronome {
    std::string tempo = "deterministic";
    int tick = 0;

    bool operator==(const metronome& rhs) {
      return
        tempo == rhs.tempo &&
        tick  == rhs.tick;
    };
  };

  struct composite_state {
    int a;
    metronome m;

    bool operator==(const composite_state& rhs) {
      return
        a == rhs.a &&
        m == rhs.m;
    };
  };
}

SCENARIO( "a State can represent a single value, collection, or composition of values" ) {

  using namespace State;

  GIVEN( "an int state and an increment action" ){
    using action_t = std::variant<TestAction::increment>;

    auto int_state = State<int>::with_reducer<action_t>
      (666,
       [] (int state, action_t action) -> int {
         return
           match(action,
                 [&state] (const TestAction::increment& a) {
                   state++;
                   return state;
                 });
       });

    WHEN( "the state is reduced with the increment action" ) {
      REQUIRE( int_state->value() == 666 );
      int_state->reduce(TestAction::increment{});

      THEN( "the int_state is incremented" ) {
        REQUIRE( int_state->value() == 667 );
      }
    }
  }
  
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
                   auto new_state = state;
                   std::transform(new_state.begin(), new_state.end(), new_state.begin(),
                                  [](unsigned char c) -> unsigned char { return std::tolower(c); });
                   return new_state;
                 });
       });

    WHEN( "the state is reduced with a change_str action" ) {
      REQUIRE ( str_state->value() == "HELLo" );
      str_state->reduce(TestAction::change_str{ .new_str = "hOly mOly" });
      
      THEN( "the state changes to the new string" ) {
        REQUIRE ( str_state->value() == "hOly mOly" );  
      }
    }

    WHEN( "the state is reduced with a lowercase_str action" ) {
      REQUIRE ( str_state->value() == "HELLo" );
      str_state->reduce(TestAction::lowercase_str{});

      THEN( "the state is changed to lowercase" ) {
        REQUIRE( str_state->value() == "hello");
      }
    }
  }

  GIVEN( "a struct State and increment & change_state actions" ) {

    using action_t = std::variant<TestAction::increment,
                                  TestAction::change_state>;

    auto struct_state = State<TestState::metronome>::with_reducer<action_t>
      (TestState::metronome{},
       [] (TestState::metronome state, action_t action) -> TestState::metronome {
         return
           match(action,
                 [&] (const TestAction::increment& a) {
                   state.tick++;
                   return state;
                 },
                 [&] (const TestAction::change_state& a) {
                   state.tempo = a.new_state;
                   return state;
                 });
       });

    WHEN( "the state is reduced with the increment action" ) {
      REQUIRE( struct_state->value().tick == 0 );
      struct_state->reduce(TestAction::increment{});

      THEN( "the state tick is incremented by one" ) {
        REQUIRE( struct_state->value().tick == 1 );
      }
    }

    WHEN( "the state is reduced with the change_state action" ) {
      REQUIRE( struct_state->value().tempo == "deterministic" );
      struct_state->reduce(TestAction::change_state{ .new_state = "swing" });

      THEN( "the state's (metronome) tempo is changed to swing" ) {
        REQUIRE( struct_state->value().tempo == "swing" );
      }
    }
  }

  GIVEN( "a vector state and append & clear actions" ) {

    using action_t = std::variant<TestAction::append,
                                  TestAction::clear>;

    auto vector_state = State<std::vector<int> >::with_reducer<action_t>
      ({4, 5, 6},
       [] (std::vector<int> state, action_t action) -> std::vector<int> {
         return
           match(action,
                 [&] (const TestAction::append& a) {
                   state.push_back(a.number);
                   return state;
                 },
                 [&] (const TestAction::clear& a) {
                   state.clear();
                   return state;
                 });
       });

    WHEN( "the state is reduced with the append action" ) {
      REQUIRE( vector_state->value().size() == 3 );
      REQUIRE( vector_state->value() == std::vector<int>({4, 5, 6}));
      vector_state->reduce(TestAction::append{ .number = 9 });

      THEN( "the vector state has a new number appended to it" ) {
        REQUIRE( vector_state->value().size() == 4 );
        REQUIRE( vector_state->value() == std::vector<int>({4, 5, 6, 9}));
      }
    }

  }

  GIVEN( "a composite state with actions overlapping to each substate" ) {

    using action_t = std::variant<TestAction::increment,
                                  TestAction::clear,
                                  TestAction::change_state>;

    auto s1 = State<int>::with_reducer<action_t>
      (666,
       [] (int state, action_t action) -> int {
         return
           match(action,
                 [&] (const TestAction::increment& a) {
                   return state + 1;
                 },
                 [&] (const TestAction::clear& a) {
                   return 0;
                 },
                 [&] (const auto& a) {  // note: for default case, make sure it is const qualified...
                   return state;
                 });
       });

    auto s2 = State<TestState::metronome>::with_reducer<action_t>
      (TestState::metronome{},
       [] (TestState::metronome state, action_t action) -> TestState::metronome {
         return
           match(action,
                 [&] (const TestAction::clear& a) {
                   state.tempo = "zero";
                   return state;
                 },
                 [&] (const TestAction::change_state& a) {
                   state.tempo = a.new_state;
                   return state;
                 },
                 [&] (const auto& a) {
                   return state;
                 });
       });

    auto c = State<TestState::composite_state>
      ::with_actions<action_t>
      ::compose<int, TestState::metronome>
      ([] (int a, TestState::metronome m) -> TestState::composite_state {
         return {
                 .a = a,
                 .m = m,
         };
       }, s1, s2);

    WHEN( "the state is reduced with the increment action" ) {
      REQUIRE( c->value().a == 666 );
      REQUIRE( c->value().m.tempo == "deterministic" );
      REQUIRE( c->value().m.tick == 0 );
      c->reduce(TestAction::increment{});

      THEN( "only the int substate is incremented" ) {
        REQUIRE( s1->value() == 667 );
        REQUIRE( c->value().a == 667 );
        REQUIRE( c->value().m.tempo == "deterministic" );
        REQUIRE( c->value().m.tick == 0 );
      }
    }

    WHEN( "the state is reduced with the clear action" ) {
      REQUIRE( c->value().a == 666 );
      REQUIRE( c->value().m.tempo == "deterministic" );
      REQUIRE( c->value().m.tick == 0 );
      c->reduce(TestAction::clear{});

      THEN( "the int substate is 0 and the metronome.tempo state is 'zero'" ) {
        REQUIRE( s1->value() == 0 );
        REQUIRE( c->value().a == 0 );
        REQUIRE( c->value().m.tempo == "zero" );
        REQUIRE( s2->value().tempo == "zero" );
        REQUIRE( c->value().m.tick == 0 );
      }
    }

    WHEN( "the state is reduced with the change_state action" ) {
      REQUIRE( c->value().a == 666 );
      REQUIRE( c->value().m.tempo == "deterministic" );
      REQUIRE( c->value().m.tick == 0 );
      c->reduce(TestAction::change_state{ .new_state = "fast"});

      THEN( "the metronome substate's tempo is 'fast'" ) {
        REQUIRE( c->value().a == 666 );
        REQUIRE( c->value().m.tempo == "fast" );
        REQUIRE( s2->value().tempo == "fast" );
        REQUIRE( c->value().m.tick == 0 );        
      }
    }
  }
}

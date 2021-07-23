#include <catch.hpp>

#include <string>
#include <memory>
#include <variant>

#include "anemone/rx/state.hpp"
#include "anemone/rx/observer.hpp"

#include "fixtures/observers.hpp"
#include "fixtures/actions.hpp"
#include "fixtures/state/composite_state.hpp"
#include "fixtures/state/int_state.hpp"
#include "fixtures/state/struct_state.hpp"


SCENARIO( "a state nodes is observable by a state observer" ) {

  GIVEN( "an observable state and a state observer" ) {

    auto s = std::make_shared<fixture::state::CompositeState>();
    auto o = fixture::observer::rx::Simple();

    o.subscribe<fixture::state::composite_state_t>
      (s,
       [&o] (fixture::state::composite_state_t state) {
         o.count = state.vector.size();
       });
    
    WHEN( "the state is updated" ) {
      REQUIRE( o.count == 0 );
      s->state->reduce(fixture::action::append{ .item = 9 });
      
      THEN( "the observer is notified" ) {
        REQUIRE( o.count == 4 );  
      }
    }
  }

  GIVEN( "two observable states and a state observer" ) {
    auto s1 = std::make_shared<fixture::state::IntState>();
    auto s2 = std::make_shared<fixture::state::StructState>();
    auto o = fixture::observer::rx::Simple();

    o.subscribe<int>
      (s1,
       [&o] (int state) {
         o.count = state - 656;
       });
    o.subscribe<fixture::state::struct_state_t>
      (s2,
       [&o] (fixture::state::struct_state_t state) {
         o.on = (o.count > 10) && state.on;
       });

    WHEN( "the both states are updated" ) {
      REQUIRE( o.count == 0 );
      REQUIRE( !o.on );
      s1->state->reduce(fixture::action::increment{});
      s2->state->reduce(fixture::action::toggle{});
      
      THEN( "the observer is notified of both state changes" ) {
        REQUIRE( o.count == 11 );
        REQUIRE( o.on );
      }
    }
  }
}

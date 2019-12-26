#include <catch.hpp>

#include <cctype>
#include <string>
#include <vector>
#include <variant>
#include <algorithm>

#include "fixtures/actions.hpp"
#include "fixtures/state/int_state.hpp"
#include "fixtures/state/struct_state.hpp"
#include "fixtures/state/string_state.hpp"
#include "fixtures/state/vector_state.hpp"
#include "fixtures/state/composite_state.hpp"


SCENARIO( "a State can represent a single value, collection, or composition of values" ) {

  GIVEN( "an int state and an increment action" ){

    auto int_node = fixture::state::IntState();
    
    WHEN( "the state is reduced with the increment action" ) {
      REQUIRE( int_node.state->get() == 666 );
      int_node.state->reduce(fixture::action::increment{});

      THEN( "the int_state is incremented" ) {
        REQUIRE( int_node.state->get() == 667 );
      }
    }
  }
  
  GIVEN( "a string State and a change_str & lowercase_str action" ) {

    auto str_node = fixture::state::StringState();
    
    WHEN( "the state is reduced with a change_str action" ) {
      REQUIRE ( str_node.state->get() == "SineWaVe" );
      str_node.state->reduce(fixture::action::change_str{ .new_str = "hOly mOly" });
      
      THEN( "the state changes to the new string" ) {
        REQUIRE ( str_node.state->get() == "hOly mOly" );  
      }
    }

    WHEN( "the state is reduced with a lowercase_str action" ) {
      REQUIRE ( str_node.state->get() == "SineWaVe" );
      str_node.state->reduce(fixture::action::lowercase{});

      THEN( "the state is changed to lowercase" ) {
        REQUIRE( str_node.state->get() == "sinewave");
      }
    }
  }

  GIVEN( "a struct State and increment & change_state actions" ) {

    auto struct_node = fixture::state::StructState();
    
    WHEN( "the state is reduced with the increment action" ) {
      REQUIRE( struct_node.state->get().number == 888 );
      struct_node.state->reduce(fixture::action::increment{});

      THEN( "the state tick is incremented by one" ) {
        REQUIRE( struct_node.state->get().number == 889 );
      }
    }

    WHEN( "the state is reduced with the toggle action" ) {
      REQUIRE( !struct_node.state->get().on );
      struct_node.state->reduce(fixture::action::toggle{});

      THEN( "the state is set to on" ) {
        REQUIRE( struct_node.state->get().on );
      }
    }
  }

  GIVEN( "a vector state and append & clear actions" ) {

    auto vector_node = fixture::state::VectorState();
    
    WHEN( "the state is reduced with the append action" ) {
      REQUIRE( vector_node.state->get().size() == 3 );
      REQUIRE( vector_node.state->get() == std::vector<int>({4, 5, 6}));
      vector_node.state->reduce(fixture::action::append{ .item = 9 });

      THEN( "the vector state has a new number appended to it" ) {
        REQUIRE( vector_node.state->get().size() == 4 );
        REQUIRE( vector_node.state->get() == std::vector<int>({4, 5, 6, 9}));
      }
    }

  }

  GIVEN( "a composite state with actions overlapping to each substate" ) {

    auto composite_node = fixture::state::CompositeState();
    
    WHEN( "the state is reduced with the increment action" ) {
      REQUIRE( composite_node.state->get().vector == std::vector<int>({4, 5, 6}));
       REQUIRE( composite_node.state->get().parameter == fixture::state::struct_state_t{});
      
      composite_node.state->reduce(fixture::action::increment{});

      THEN( "only the struct substate number field is incremented" ) {
        REQUIRE( composite_node.struct_substate.state->get().number == 889 );
        REQUIRE( composite_node.state->get().parameter.number == 889 );
        REQUIRE( !composite_node.state->get().parameter.on );
        REQUIRE( composite_node.state->get().vector == std::vector<int>({4, 5, 6}));
      }
    }

    WHEN( "the state is reduced with the celar action" ) {
      REQUIRE( composite_node.state->get().vector == std::vector<int>({4, 5, 6}));
      REQUIRE( composite_node.state->get().parameter == fixture::state::struct_state_t{});
      
      composite_node.state->reduce(fixture::action::clear{});

      THEN( "the vector substate is now empty" ) {
        REQUIRE( composite_node.state->get().parameter == fixture::state::struct_state_t{});
        REQUIRE( composite_node.vector_substate.state->get() == std::vector<int>({}) );
        REQUIRE( composite_node.state->get().vector == std::vector<int>({}) );
      }
    }
  }
}

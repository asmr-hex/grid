#include <catch.hpp>

#include "anemone/io/grid/coordinates.hpp"


SCENARIO( "grid_coordinates_t can be compared to one another" ) {

  GIVEN( "two different grid_coordinates_t values where c1 > c0" ) {
    grid_coordinates_t c0 = { .x = 0, .y = 0 };
    grid_coordinates_t c1 = { .x = 444, .y = 666 };

    WHEN( "they are == compared" ) {
      THEN( "they are evaluated as not equal" ) {
        REQUIRE( !(c0 == c1) );
      }
    }

    WHEN( "they are < compared" ) {
      THEN( "c1 is evaluated as greater than c0" ) {
        REQUIRE( c0 < c1 );
      }
    }
  }

  GIVEN( "two equal grid_coordinates_t values" ) {
    grid_coordinates_t c0 = { .x = 444, .y = 666 };
    grid_coordinates_t c1 = { .x = 444, .y = 666 };

    WHEN( "they are == compared" ) {
      THEN( "they are evaluated as equal" ) {
        REQUIRE( c0 == c1 );
      }
    }

    WHEN( "they are < compared" ) {
      THEN( "c1 is evaluated as not greater than c0" ) {
        REQUIRE( !(c0 < c1) );
      }
    }
  }
}

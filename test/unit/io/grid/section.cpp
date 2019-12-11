#include <catch.hpp>

#include "anemone/io/grid/section.hpp"


SCENARIO( "we need to know details about sections of the Grid" ) {
  grid_region_t region = { .min = { .x = 0, .y = 0}, .max = { .x = 15, .y = 1} };
  GridSectionName name = GridSectionName::Steps;

  GIVEN( "a GridSection" ) {
    auto section = GridSection(name, region);

    WHEN( "width() is called" ) {
      THEN( "it returns the proper width" ) {
        REQUIRE( section.width() == 16 );
      }
    }

    WHEN( "height() is called" ) {
      THEN( "it returns the proper height" ) {
        REQUIRE( section.height() == 2 );
      }
    }

    WHEN( "size() is called" ) {
      THEN( "it returns the proper size" ) {
        REQUIRE( section.size() == 32 );
      }
    }

    WHEN( "region() is called" ) {
      grid_region_t result = section.region();
      
      THEN( "it returns the proper region" ) {
        REQUIRE( result.min == region.min );
        REQUIRE( result.max == region.max );
      }
    }

    WHEN( "coordinates_of(...) is called" ) {
      grid_coordinates_t expected = { .x = 4, .y = 1 };
      grid_coordinates_t result = section.coordinates_of(20);
      
      THEN( "it returns the proper coordinates" ) {
        REQUIRE( result == expected );
      }
    }

    WHEN( "coordinates_in(...) is called" ) {
      std::vector<grid_coordinates_t> expected = {
                                                  { .x = 0, .y = 0 },
                                                  { .x = 1, .y = 0 },
                                                  { .x = 2, .y = 0 },
      };
      std::vector<grid_coordinates_t> result = section.coordinates_in(0, 2);
      
      THEN( "it returns the proper coordinates" ) {
        REQUIRE( result == expected );
      }
    }

    WHEN( "index_of(...) is called" ) {
      grid_coordinates_t coordinates = { .x = 4, .y = 1 };
      
      THEN( "it returns the proper coordinates" ) {
        REQUIRE( section.index_of(coordinates) == 20 );
      }
    }
  }
}

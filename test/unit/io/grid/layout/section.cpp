#include <catch.hpp>

#include "anemone/io/grid/layout/section.hpp"


SCENARIO( "we need to know details about sections of the Grid" ) {
  GIVEN( "a GridSection" ) {
    grid_region_t region = { .min = { .x = 0, .y = 0}, .max = { .x = 15, .y = 1} };
    GridSectionName name = GridSectionName::Steps;

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

    WHEN( "contains(...) is called on a coordinate within the section" ) {
      grid_coordinates_t coordinates = { .x = 1, .y = 0 };
      
      THEN( "it returns true") {
        REQUIRE( section.contains(coordinates) );
      }
    }

    WHEN( "contains(...) is called on a coordinate outside of the section" ) {
      grid_coordinates_t coordinates = { .x = 1, .y = 5 };
      
      THEN( "it returns false") {
        REQUIRE( !section.contains(coordinates) );
      }
    }
    
    WHEN( "index_of(...) is called" ) {
      grid_coordinates_t coordinates = { .x = 4, .y = 1 };
      
      THEN( "it returns the proper coordinates" ) {
        REQUIRE( section.index_of(coordinates) == 20 );
      }
    }
  }

  GIVEN( "another GridSection" ) {
    grid_region_t region = { .min = { .x = 0, .y = 0}, .max = { .x = 2, .y = 0} };
    GridSectionName name = GridSectionName::Steps;

    auto section = GridSection(name, region);

    WHEN( "coordinates_in() is called" ) {
      std::vector<grid_coordinates_t> expected = {
                                                  { .x = 0, .y = 0 },
                                                  { .x = 1, .y = 0 },
                                                  { .x = 2, .y = 0 },
      };
      std::vector<grid_coordinates_t> result = section.coordinates_in();
      
      THEN( "it returns all coordinates within the GridSection" ) {
        REQUIRE( result == expected );
      }
    }
  }
}

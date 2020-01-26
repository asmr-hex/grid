#include <catch.hpp>

#include <iostream>

#include "fixtures/integration/globals.hpp"
#include "anemone/io.hpp"


SCENARIO( "Activating and Deactivating Steps" ) {
  std::cout << test_grid_device <<std::endl;
  GIVEN("an empty step sequence") {
    WHEN("a step is pressed") {
      grid_addr_t step = { .layout = LayoutName::SequencerAndInstrument,
                           .section = GridSectionName::Steps,
                           .index = 3,
      };

      test_grid_device->toggle(step);
      THEN("its corresponding led turns on") {
        REQUIRE( test_grid_device->check_led_level(step) == 15 );
      }
    }
  }
}

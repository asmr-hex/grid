#include <catch.hpp>

#include "fixtures/integration/globals.hpp"
#include "anemone/io.hpp"


SCENARIO("Activating and Deactivating Steps") {
  
  GIVEN("an empty step sequence") {
    grid_addr_t step = { .layout = LayoutName::SequencerAndInstrument,
                         .section = GridSectionName::Steps,
                         .index = 3,
    };

    WHEN("an unactivated step is pressed") {
      test_grid_device->describe("pressing a step down activates it");
      test_grid_device->toggle(step);
      THEN("its corresponding led turns on") {
        REQUIRE( test_grid_device->check_led_level(step) == 15 );          
      }
    }

    WHEN("the same step is unpressed") {
      test_grid_device->toggle(step);
      THEN("it has no effect") {
        REQUIRE( test_grid_device->check_led_level(step) == 15 );          
      }
    }

    WHEN("the cursor moves over an activated step") {
      test_grid_device->wait();
      test_grid_device->describe("when the cursor moves onto an activated step, a midi note plays!");
      // TODO include some wait logic here....?
      auto results = test_utility->record_step_outputs({3});
      THEN("a midi note is played") {
        
      }
    }
    
    WHEN("an activated step is pressed") {
      test_grid_device->wait();
      test_grid_device->describe("pressing the step again deactivates it");
      test_grid_device->toggle(step);
      THEN("it has no effect") {
        REQUIRE( test_grid_device->check_led_level(step) == 0 );          
      }
    }

    WHEN("the same step is unpressed") {
      test_grid_device->toggle(step);
      THEN("it has no effect") {
        REQUIRE( test_grid_device->check_led_level(step) == 0 );          
      }
    }
  }
}

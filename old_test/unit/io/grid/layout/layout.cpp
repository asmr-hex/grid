#include <memory>
#include <vector>

#include <catch.hpp>

#include "fixtures/observers.hpp"
#include "fixtures/layouts.hpp"

#include "anemone/io/grid/layout/context.hpp"
#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/event.hpp"


SCENARIO( "a grid Layout delegates events to its constituent sections" ) {
  GIVEN( "a layout" ) {
    TestSequencerLayout layout;

    WHEN( "the layout translates a grid address" ) {
      grid_addr_t addr = { .layout = LayoutName::SequencerAndInstrument,
                           .section = GridSectionName::Steps,
                           .index = 30
      };
      grid_coordinates_t result = layout.translate(addr);
      grid_coordinates_t expected = { .x = 14, .y = 1 };
  
      THEN( "it returns the correct coordinates" ) {
        REQUIRE( result == expected );
      }
    }
  }

  GIVEN( "a layout and observers subscribed to its sections" ) {
    TestSequencerLayout layout;
    LayoutContext ctx({std::make_shared<TestSequencerLayout>()}); // dummy ctx for the sake of this test
    
    auto observer_a = std::make_shared<TestGridEventObserver>();
    auto observer_b = std::make_shared<TestGridEventObserver>();
    
    observer_a->subscribe(*layout.steps);
    observer_b->subscribe(*layout.play_pause);
    
    WHEN( "the layout calls notify" ) {
      std::vector<grid_device_event_t> device_events =
        {
         grid_device_event_t::make(0, 2, GridDeviceEvent::ButtonDown),
         grid_device_event_t::make(0, 2, GridDeviceEvent::ButtonUp),
         grid_device_event_t::make(6, 1, GridDeviceEvent::ButtonDown),
         grid_device_event_t::make(6, 1, GridDeviceEvent::ButtonUp),
        };

      std::vector<grid_event_t> expected_a =
        {
         grid_event_t::make(LayoutName::SequencerAndInstrument,
                            GridSectionName::Steps,
                            22,
                            GridEvent::Pressed),
         grid_event_t::make(LayoutName::SequencerAndInstrument,
                            GridSectionName::Steps,
                            22,
                            GridEvent::Unpressed),
        };
      
      std::vector<grid_event_t> expected_b =
        {
         grid_event_t::make(LayoutName::SequencerAndInstrument,
                            GridSectionName::PlayPause,
                            0,
                            GridEvent::Pressed),
         grid_event_t::make(LayoutName::SequencerAndInstrument,
                            GridSectionName::PlayPause,
                            0,
                            GridEvent::Unpressed),
        };
      
      for (auto e : device_events) {
        layout.notify(ctx, e);  
      }

      THEN( "the observers get notified" ) {
        REQUIRE( observer_b->events.size() == 2 );
        REQUIRE( observer_b->events == expected_b );
        REQUIRE( observer_a->events.size() == 2 );
        REQUIRE( observer_a->events == expected_a );
      }
    }    
  }
}

#include <memory>
#include <vector>
#include <iostream>

#include <catch.hpp>

#include "anemone/io/observer.hpp"

#include "anemone/io/grid/layout/context.hpp"
#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/names.hpp"


class TestLayoutObserver : public Observer<grid_event_t> {
public:
  virtual void handle(const grid_event_t& event) override {
    events.push_back(event);
  }

  std::vector<grid_event_t> events;
};


// derive a small test layout
class TestLayout : public Layout {
public:
  GridSection steps;
  GridSection play_pause;

  TestLayout() : Layout(LayoutName::SequencerAndInstrument) {
    steps = GridSection(GridSectionName::Steps,
                        { .min = { .x = 0, .y = 0},
                          .max = { .x = 15, .y = 1} });
    play_pause = GridSection(GridSectionName::PlayPause,
                             { .min = { .x = 0, .y = 2},
                               .max = { .x = 0, .y = 2} });

    register_section(steps);
    register_section(play_pause);
  };
};


SCENARIO( "a grid Layout delegates events to its constituent sections" ) {
  GIVEN( "a layout" ) {
    TestLayout layout;

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
    TestLayout layout;
    LayoutContext ctx({std::make_shared<TestLayout>(layout)});
    
    auto observer_a = std::make_shared<TestLayoutObserver>();
    auto observer_b = std::make_shared<TestLayoutObserver>();
    
    observer_a->subscribe(layout.steps);
    observer_b->subscribe(layout.play_pause);
    
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

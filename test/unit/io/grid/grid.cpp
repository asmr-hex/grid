#include <catch.hpp>

#include "mocks/device/grid.hpp"
#include "fixtures/layouts.hpp"
#include "fixtures/observers.hpp"
#include "fixtures/configs/config.hpp"

#include "anemone/config/config.hpp"
#include "anemone/io/grid/grid.hpp"
#include "anemone/io/grid/layout/names.hpp"


SCENARIO( "a Grid handles events from a GridDevice and propogates grid events" ) {
  GIVEN( "a grid device, two layouts, and a Grid" ) {
    auto sqncr_layout = std::make_shared<TestSequencerLayout>();
    auto p_sqncr_layout = std::make_shared<TestParallelSequencerLayout>();
    auto grid_device = std::make_shared<MockGridDevice>();
    auto config = std::make_shared<Config>(fixture::config::default_conf);
    auto grid = std::make_shared<Grid>(Grid(config, grid_device, {sqncr_layout, p_sqncr_layout}));

    WHEN( "the Grid is constructed" ) {
      THEN( "the layout is correctly set" ) {
        REQUIRE( grid->layout.name() == LayoutName::SequencerAndInstrument );
      }
    }

    WHEN( "the Grid connects and a device event occurs" ) {
      grid->connect();

      THEN( "the Grid receives the event from the grid device" ) {
        REQUIRE( grid->layout.name() == LayoutName::SequencerAndInstrument );
        grid_device->emit(grid_device_event_t::make(15, 7, GridDeviceEvent::ButtonDown));
        REQUIRE( grid->layout.name() == LayoutName::ParallelSequencer );
      }
    }

    WHEN( "the Grid connects, there are observers, and a device event occurs" ) {
      auto observer = std::make_shared<TestGridEventObserver>();
      observer->subscribe(sqncr_layout->switch_layout);

      auto expected = grid_event_t::make(LayoutName::SequencerAndInstrument,
                                         GridSectionName::SwitchLayout,
                                         0,
                                         GridEvent::Pressed);
      
      grid->connect();
      grid_device->emit(grid_device_event_t::make(15, 7, GridDeviceEvent::ButtonDown));
      
      THEN( "the Grid receives the event from the grid device" ) {
        REQUIRE( observer->events.size() == 1 );
        REQUIRE( observer->events[0] == expected );
      }
    }
  }
}


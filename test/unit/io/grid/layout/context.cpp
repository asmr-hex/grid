#include <catch.hpp>

#include "fixtures/observers.hpp"
#include "fixtures/layouts.hpp"

#include "anemone/io/grid/layout/context.hpp"
#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/address.hpp"


SCENARIO( "the LayoutContext delegates to its current layout and can switch layouts" ) {
  GIVEN( "two layouts and a layout context" ) {
    auto sqncr_layout = std::make_shared<TestSequencerLayout>();
    auto p_sqncr_layout = std::make_shared<TestParallelSequencerLayout>();
    LayoutContext ctx({sqncr_layout, p_sqncr_layout});

    WHEN( "the LayoutContext is initialized" ) {
      THEN( "the first layout argument becomes the current layout" ) {
        REQUIRE( ctx.name() == LayoutName::SequencerAndInstrument );
      }
    }
    
    WHEN( "the LayoutContext calls translate" ) {
      grid_addr_t addr =
        {
         .layout  = LayoutName::SequencerAndInstrument,
         .section = GridSectionName::Steps,
         .index   = 20,
        };

      THEN( "it delegates the translation to the appropriate layout/section" ) {
        grid_coordinates_t expected = { .x = 4, .y = 1 };
        REQUIRE( ctx.translate(addr) == expected );
      }
    }
  }
  
  GIVEN( "two layouts with subscribed observers and a layout context" ) {
    auto sqncr_layout = std::make_shared<TestSequencerLayout>();
    auto p_sqncr_layout = std::make_shared<TestParallelSequencerLayout>();
    LayoutContext ctx({sqncr_layout, p_sqncr_layout});

    auto observer_a = std::make_shared<TestGridEventObserver>();
    auto observer_b = std::make_shared<TestGridEventObserver>();
    auto observer_c = std::make_shared<TestGridEventObserver>();
    auto observer_d = std::make_shared<TestGridEventObserver>();


    observer_a->subscribe(*sqncr_layout->steps);
    observer_b->subscribe(*p_sqncr_layout->steps);
    observer_c->subscribe(*sqncr_layout->switch_layout);
    observer_d->subscribe(*p_sqncr_layout->switch_layout);


    WHEN( "the LayoutContext is notified of a grid device event" ) {
      auto device_event =
        grid_device_event_t::make(3, 1, GridDeviceEvent::ButtonDown);
      auto expected =
        grid_event_t::make(LayoutName::SequencerAndInstrument,
                           GridSectionName::Steps,
                           19,
                           GridEvent::Pressed);
        
      ctx.notify(device_event);

      THEN( "the current layout/section translates to grid event & notifies observers" ) {
        REQUIRE( observer_a->events.size() == 1 );
        REQUIRE( observer_b->events.size() == 0 );
        REQUIRE( observer_a->events[0] == expected );
      }
    }

    WHEN( "the LayoutContext is notified of an switch layout event" ) {
      auto device_event =
        grid_device_event_t::make(15, 7, GridDeviceEvent::ButtonDown);
      auto expected =
        grid_event_t::make(LayoutName::SequencerAndInstrument,
                           GridSectionName::SwitchLayout,
                           0,
                           GridEvent::Pressed);
      
      THEN( "the current layout is switched and the old layout broadcasts the switch layout event to observers" ) {
        REQUIRE( ctx.name() == LayoutName::SequencerAndInstrument );
        ctx.notify(device_event);
        REQUIRE( ctx.name() == LayoutName::ParallelSequencer );
        REQUIRE( observer_c->events.size() == 1 );
        REQUIRE( observer_c->events[0] == expected );
      }
    }
  }
}

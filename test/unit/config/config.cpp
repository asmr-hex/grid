#include <catch.hpp>

#include "anemone/config/config.hpp"

#include "fixtures/configs/config.hpp"


SCENARIO( "a Config can read in a configuration file and parse it" ) {

  GIVEN( "a config file" ) {
    WHEN( "a config object is instantiated with the config file" ) {
      auto config = Config(fixture::config::default_conf);
      
      THEN( "the config object is populated correctly" ) {
        REQUIRE( config.layouts.sequencer.instrument_select.region()
                 == grid_region_t{.min = { .x = 7, .y = 2 },
                                  .max = { .x = 7, .y = 7 }} );

        REQUIRE( config.layouts.sequencer.instrument_panel.region()
                 == grid_region_t{.min = { .x = 8, .y = 2 },
                                  .max = { .x = 15, .y = 7 }} );

        REQUIRE( config.layouts.sequencer.steps.region()
                 == grid_region_t{.min = { .x = 0, .y = 0 },
                                  .max = { .x = 15, .y = 1 }} );

        REQUIRE( config.layouts.sequencer.pages.region()
                 == grid_region_t{.min = { .x = 1, .y = 2 },
                                  .max = { .x = 6, .y = 2 }} );

        REQUIRE( config.layouts.sequencer.parts.region()
                 == grid_region_t{.min = { .x = 0, .y = 3 },
                                  .max = { .x = 6, .y = 3 }} );

        REQUIRE( config.layouts.sequencer.banks.region()
                 == grid_region_t{.min = { .x = 0, .y = 4 },
                                  .max = { .x = 6, .y = 4 }} );

        REQUIRE( config.layouts.sequencer.ppqn.region()
                 == grid_region_t{.min = { .x = 0, .y = 5 },
                                  .max = { .x = 6, .y = 5 }} );

        REQUIRE( config.layouts.sequencer.shift.region()
                 == grid_region_t{.min = { .x = 0, .y = 7 },
                                  .max = { .x = 0, .y = 7 }} );

        REQUIRE( config.layouts.sequencer.play_pause.region()
                 == grid_region_t{.min = { .x = 1, .y = 7 },
                                  .max = { .x = 1, .y = 7 }} );

        REQUIRE( config.layouts.sequencer.stop.region()
                 == grid_region_t{.min = { .x = 2, .y = 7 },
                                  .max = { .x = 2, .y = 7 }} );

        REQUIRE( config.layouts.sequencer.record.region()
                 == grid_region_t{.min = { .x = 2, .y = 7 },
                                  .max = { .x = 2, .y = 7 }} );

        REQUIRE( config.layouts.sequencer.last_step.region()
                 == grid_region_t{.min = { .x = 0, .y = 2 },
                                  .max = { .x = 0, .y = 2 }} );

        REQUIRE( config.layouts.sequencer.metronome.region()
                 == grid_region_t{.min = { .x = 6, .y = 7 },
                                  .max = { .x = 6, .y = 7 }} );
      }
    }
  }
}

#include <catch.hpp>

#include "anemone/config/config.hpp"

#include "fixtures/configs/config.hpp"


SCENARIO( "a Config can read in a configuration file and parse it" ) {

  GIVEN( "a config file" ) {
    WHEN( "a config object is instantiated with the config file" ) {
      auto config = Config(fixture::config::default_conf);
      
      THEN( "the config object is populated correctly" ) {
        
      }
    }
  }
}

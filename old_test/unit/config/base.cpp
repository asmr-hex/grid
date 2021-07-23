#include <catch.hpp>

#include <string>

#include "anemone/config/base.hpp"

#include "fixtures/configs/config.hpp"


SCENARIO( "a Base config reads in a file and nodes can be accessed via [] operator" ) {

  GIVEN( "a Base config and a config file" ) {
    Conf::Base config(fixture::config::simple);

    WHEN( "the config accesses the 'layout' yaml node via the [] operator" ) {
      Conf::Base result = config["layout"];

      THEN( "it returns the correct YAML node" ) {
        REQUIRE( result["key"].as<std::string>() == "value" );
      }
    }
  }
}


SCENARIO( "a Base config can read in another yaml file if a field has '_file' appened" ) {

  GIVEN( "a Base config and a config file with a nested layout yaml file" ) {
    Conf::Base config(fixture::config::nested);

    WHEN( "the config accesses the 'layout' node" ) {
      Conf::Base result = config["layout"];

      THEN( "the proper layout yml node is returned" ) {
        REQUIRE( result["layout"]["key"].as<std::string>() == "value" );
      }
    }
  }
}



#include <catch.hpp>

#include <string>

#include "anemone/config/base.hpp"

#include "fixtures/configs/config.hpp"


SCENARIO( "a Base config reads in a file and nodes can be accessed via [] operator" ) {

  GIVEN( "a Base config and a config file" ) {
    Conf::Base config(fixture::config::simple);

    WHEN( "the config accesses the 'layout' yaml node via the [] operator" ) {
      YAML::Node result = config["layout"];

      THEN( "it returns the correct YAML node" ) {
        REQUIRE( result["key"].as<std::string>() == "value" );
      }
    }
  }
}


SCENARIO( "a Base config can parse parent directories from file paths" ) {

  GIVEN( "a Base config and a file path" ) {
    std::string file_path = "conf/parent_dir/config.yml";
    Conf::Base config(fixture::config::simple);
    
    WHEN( "the config calls get_parent_dir on the file path" ) {
      std::string result = config.get_parent_dir(file_path);
      
      THEN( "the parent directory is returned" ) {
        REQUIRE( result == "conf/parent_dir/" );
      }
    }
  }
}


SCENARIO( "a Base config can get layout yaml nodes from a config file" ) {

  GIVEN( "a Base config and a config file with an inline layout yaml node" ) {
    Conf::Base config(fixture::config::simple);

    WHEN( "the config calls get_layout" ) {
      YAML::Node result = config.get_layout(config.yml);

      THEN( "the proper layout yml node is returned" ) {
        REQUIRE( result["key"].as<std::string>() == "value" );
      }
    }
  }

  GIVEN( "a Base config and a config file with a nested layout yaml file" ) {
    Conf::Base config(fixture::config::nested);

    WHEN( "the config calls get_layout" ) {
      YAML::Node result = config.get_layout(config.yml);

      THEN( "the proper layout yml node is returned" ) {
        REQUIRE( result["layout"]["key"].as<std::string>() == "value" );
      }
    }
  }
}



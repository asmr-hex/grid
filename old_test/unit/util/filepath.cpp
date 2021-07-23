#include <catch.hpp>

#include "anemone/util/filepath.hpp"


SCENARIO( "filepath::get_parent_dir can extract parent directories from file path strings" ) {

  GIVEN( "a file path string" ) {
    std::string file_path = "conf/parent_dir/config.yml";

    WHEN( "filepath::get_parent_dir is called on the file path string" ) {
      std::string result = filepath::get_parent_dir(file_path);
      
      THEN( "the parent directory is returned" ) {
        REQUIRE( result == "conf/parent_dir/" );
      }
    }
  }
}


#include <catch2/catch.hpp>

#include <opr/modules>

using namespace opr;


TEST_CASE( "opr::module::Output<protocol::Midi>(...)" ) {
  SECTION( "()" ) {
    GIVEN( "nothing" ) {
      opr::module::Output<protocol::Midi> out;
      WHEN( "..." ) {
        THEN( "the output is initialized" ) {}
      }
    }
  }
}

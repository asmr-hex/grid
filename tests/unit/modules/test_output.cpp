#include <catch2/catch.hpp>

#include <opr/modules>

using namespace opr;


TEST_CASE( "opr::module::Output<protocol::Midi>(...)" ) {
  SECTION( "(midi_port_t)" ) {
    GIVEN( "a valid midi port name (string)" ) {
      midi_port_t port_name = "something";
      WHEN( "a midi output is instantiated" ) {
        auto out = opr::module::Output<protocol::Midi>(port_name);
        THEN( "the output is initialized properly" ) {}
      }
    }
  }
}

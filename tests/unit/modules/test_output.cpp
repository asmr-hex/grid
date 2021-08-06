#include <catch2/catch.hpp>

#include "mocks/device/midi.hpp"

#include <opr/modules>
#include <opr/impl/midi.hpp>

using namespace opr;
using namespace opr::module;


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
  SECTION( "(std::shared_ptr<Output<protocol::Midi>::Device>)" ) {
    GIVEN( "a mock output midi device" ) {
      std::shared_ptr<Output<protocol::Midi>::Device> device(new mock::Output<protocol::Midi>::Device());
      WHEN( "a midi output is instantiated" ) {
        auto out = opr::module::Output<protocol::Midi>(device);
        THEN( "the output is initialized properly" ) {}
      }
    }
    GIVEN( "an rtmidi output midi device" ) {
        std::shared_ptr<Output<protocol::Midi>::Device::Manager> deviceManager(new opr::impl::Output<protocol::Midi>::Device::Manager::RtMidi());
        WHEN( "a midi output is instantiated" ) {
            THEN( "the output is initialized properly" ) {}
            REQUIRE( deviceManager->list().size() == 1 );
            REQUIRE( deviceManager->list()[0] == "haha" );
        }
    }
  }
}

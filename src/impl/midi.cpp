#include <opr/impl/midi.hpp>
#include <spdlog/spdlog.h>

using namespace opr;
using namespace opr::impl;


void opr::impl::Output<protocol::Midi>::Device::RtMidi::connect() {
}

void opr::impl::Output<protocol::Midi>::Device::RtMidi::emit(opr::midi_data_t data) {
}

std::vector<opr::device_name_t> opr::impl::Output<protocol::Midi>::Device::Manager::RtMidi::list() {
    // construct a new rt midi in object
    try {
        midi = std::make_unique<RtMidiOut>();
    } catch ( RtMidiError &error ) {
        error.printMessage();
        exit( EXIT_FAILURE );
    }

    // make sure there are ports available
    unsigned int n_ports = midi->getPortCount();
    if (n_ports == 0) {
        spdlog::error("no midi input ports available");
        exit( EXIT_FAILURE );
    }

    std::vector<opr::device_name_t> devices;
    for (unsigned int i = 0; i < n_ports; i++) {
        devices.push_back(midi->getPortName(i));
    }

    return devices;
}

std::shared_ptr<opr::module::Output<protocol::Midi>::Device>
opr::impl::Output<protocol::Midi>::Device::Manager::RtMidi::get(opr::device_name_t name) {
}

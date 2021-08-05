#include <opr/impl/midi.hpp>

using namespace opr;
using namespace opr::module::impl;


RtMidiOutputDevice::RtMidiOutputDevice(midi_port_t port) {
    // TODO implement
}

void RtMidiOutputDevice::connect() {
    // TODO implement
}

void RtMidiOutputDevice::emit(midi_data_t data) {
    // TODO implement
}

// fruit::Component<module::Output<protocol::Midi>::Device> getOutputMidiDevice(midi_port_t port) {
//     return fruit::createComponent()
//         .bind<module::Output<protocol::Midi>::Device, RtMidiOutputDevice>()
//         .registerConstructor<RtMidiOutputDevice(midi_port_t)>();
// }

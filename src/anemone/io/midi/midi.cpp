#include "anemone/io/midi/midi.hpp"

Midi::Midi(std::shared_ptr<Config> config, std::shared_ptr<MidiDevice> device)
  : device(device),
    ports({ .in = config->ports.midi.in, .out = config->ports.midi.out }) {}

void Midi::connect() {
  subscribe(*device);
  device->connect_in(ports.in);
  device->connect_out(ports.out);
}

void Midi::handle(const midi_event_t& event) {
  broadcast(event);
}

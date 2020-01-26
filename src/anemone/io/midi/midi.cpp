#include "anemone/io/midi/midi.hpp"

Midi::Midi(std::shared_ptr<Config> config, std::shared_ptr<MidiDeviceFactory> device_factory)
  : device_factory(device_factory) {

  // create input midi devices
  for (auto device_name : config->ports.midi.in) {
    input_devices[device_name] = device_factory->make();
  }

  // create output midi devices
  for (auto device_name : config->ports.midi.out) {
    output_devices[device_name] = device_factory->make();
  }
}

void Midi::connect() {
  // subscribe and connect to all input devices
  for (auto itr : input_devices) {
    auto device_name = itr.first;
    auto device = itr.second;

    subscribe(*device);
    device->connect_in(device_name);
  }

  // connect to all output devices
  for (auto itr : output_devices) {
    auto device_name = itr.first;
    auto device = itr.second;

    device->connect_out(device_name);    
  }
}

void Midi::handle(const midi_event_t& event) {
  broadcast(event);
}

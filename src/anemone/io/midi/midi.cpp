#include <spdlog/spdlog.h>

#include "anemone/io/midi/midi.hpp"


Midi::Midi(std::shared_ptr<Config> config, std::shared_ptr<MidiDeviceFactory> device_factory)
  : device_factory(device_factory)
{
  make_input_devices({config->ports.midi.in});
  make_output_devices({config->ports.midi.out});
}

rx::observable<midi_event_t> Midi::connect() {
  // connect to all input devices
  // for (auto itr : input_devices) {
  //   auto name = itr.first;
  //   auto device = itr.second;

  //   std::thread t
  //     ([name, device] {
  //        device->connect();
  //        device->listen();
  //      });
  //   t.detach();
  // }

  // // connect to all input devices
  // for (auto itr : output_devices) {
  //   auto name = itr.first;
  //   auto device = itr.second;
  //   std::thread t
  //     ([name, &device] {
  //        device->connect();
  //      });
  //   t.detach();
  // }

  return incoming_events.get_observable();
}

void Midi::emit(midi_event_t event) {
  
}

void Midi::emit(std::vector<midi_event_t>& events) {
  
}

void Midi::make_input_devices(std::vector<std::string> names) {
  for (auto name : names) {
    input_devices[name] = device_factory->make_input(name, incoming_events.get_subscriber());
  }
}

void Midi::make_output_devices(std::vector<std::string> names) {
  for (auto name : names) {
    output_devices[name] = device_factory->make_output(name);
  }  
}

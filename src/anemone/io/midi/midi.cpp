#include "anemone/io/midi/midi.hpp"
#include "anemone/util/wait.hpp"

#include <thread>
#include <spdlog/spdlog.h>


Midi::Midi(std::shared_ptr<Config> config, std::shared_ptr<MidiDeviceFactory> device_factory)
  : device_factory(device_factory),
    input_device_names({config->ports.midi.in}),
    output_device_names({config->ports.midi.out}) {}

void Midi::connect() {
  // // subscribe and connect to all input devices
  for (auto device_name : input_device_names) {
    // continually attemp to connect in a seperate thread until successful
    std::thread t
      ([this, device_name] { connect_with_retry(device_name, true); });
    t.detach();
  }

  // connect to all output devices
  for (auto device_name : output_device_names) {
    // continually attemp to connect in a seperate thread until successful
    std::thread t
      ([this, device_name] { connect_with_retry(device_name, false); });
    t.detach();
  }
}

void Midi::connect_with_retry(std::string device_name, bool is_input_conn) {
  bool first_attempt = true;
  bool connected = false;
  
  while (!connected) {
    // create device with factory
    auto device = device_factory->make();
    
    // try to connect
    try {
      if (is_input_conn) {
        device->connect_in(device_name);
      } else {
        device->connect_out(device_name);        
      }
      
      // at this point we are connected.
      
      connected = true;

      // include connection to map of connections
      if (is_input_conn) {
        input_devices[device_name] = device;        
      } else {
        output_devices[device_name] = device;
      }

      // subscribe to device
      subscribe(*device);
    } catch (UnavailableMidiDevice e) {
      if (first_attempt) {
        spdlog::warn(e.what());
        spdlog::warn(e.suggest());
        first_attempt = false;
      } else {
        spdlog::warn("retrying connection -> {}", device_name);
      }
      wait_for(1000);
    }
  }
}

void Midi::handle(const midi_event_t& event) {
  broadcast(event);
}

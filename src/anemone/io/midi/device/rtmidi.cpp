#include <spdlog/spdlog.h>

#include "anemone/io/midi/device/rtmidi.hpp"


RTMidiIn::RTMidiIn(std::string device_name, rx::subscriber<midi_event_t> input_stream)
  : input_stream(input_stream),
    device_name(device_name)
{}

void RTMidiIn::connect() {
  bool connected = false;
  unsigned int port;

  while (!connected) {
    // construct a new rt midi in object
    try {
      input = std::make_unique<RtMidiIn>();
    } catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }
    
    // get connected midi in devices
    auto connected_devices = list_devices();

    try {
      // get the port number for this device.
      port = connected_devices.at(device_name);
      connected = true;
    } catch (std::out_of_range &error) {
      // the device must not be connected... wait and retry.
      input.reset();
      
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      spdlog::warn("  retrying connection -> midi in -> {}...", device_name);
    }
  }

  // okay, the device has been connected and detected.
  input->openPort(port);

  spdlog::info("  connected -> midi in -> {}", device_name);
}

std::map<std::string, unsigned int> RTMidiIn::list_devices() {
  // make sure there are ports available
  unsigned int n_ports = input->getPortCount();
  if (n_ports == 0) {
    spdlog::error("no midi input ports available");
    exit( EXIT_FAILURE );
  }

  std::map<std::string, unsigned int> devices;

  // construct device -> port number map
  for (unsigned int i = 0; i < n_ports; i++) {
    try {
      devices[input->getPortName(i)] = i;
    } catch (RtMidiError &error) {
      error.printMessage();
    }
  }

  return devices;
}

void RTMidiIn::listen() {
  input->setCallback([] (double deltatime, std::vector<unsigned char> *msg, void *user_data) {
                       RTMidiIn *this_rtmidi = (RTMidiIn *)user_data;

                       midi_event_t event = { .source = this_rtmidi->name(),
                                              .destination = "",
                                              .data = *msg,
                       };

                       this_rtmidi->input_stream.on_next(event);
                     }, this);
}

std::string RTMidiIn::name() {
  return device_name;
}

// --- Midi Output ---

RTMidiOut::RTMidiOut(std::string device_name)
  : device_name(device_name)
{}

void RTMidiOut::connect() {
  bool connected = false;
  unsigned int port;

  while (!connected) {
    // construct a new rt midi in object
    try {
      output = std::make_unique<RtMidiOut>();
    } catch ( RtMidiError &error ) {
      error.printMessage();
      exit( EXIT_FAILURE );
    }

    // get connected midi in devices
    auto connected_devices = list_devices();

    try {
      // get the port number for this device.
      port = connected_devices.at(device_name);
      connected = true;
    } catch (std::out_of_range &error) {
      // the device must not be connected... wait and retry.
      output.reset();
      
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));

      spdlog::warn("  retrying connection -> midi out -> {}...", device_name);
    }
  }

  // okay, the device has been connected and detected.
  output->openPort(port);

  spdlog::info("  connected -> midi out -> {}", device_name);  
}

std::map<std::string, unsigned int> RTMidiOut::list_devices() {
  // make sure there are ports available
  unsigned int n_ports = output->getPortCount();
  if (n_ports == 0) {
    spdlog::error("no midi input ports available");
    exit( EXIT_FAILURE );
  }

  std::map<std::string, unsigned int> devices;

  // construct device -> port number map
  for (unsigned int i = 0; i < n_ports; i++) {
    try {
      devices[output->getPortName(i)] = i;
    } catch (RtMidiError &error) {
      error.printMessage();
    }
  }

  return devices;  
}

void RTMidiOut::emit(midi_event_t event) {
  output->sendMessage(&event.data);
}

std::string RTMidiOut::name() {
  return device_name;
}

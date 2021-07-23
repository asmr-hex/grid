#include "doubles/device/midi.hpp"


BrowserMidiDevice::BrowserMidiDevice(test_device::Mode test_mode,
                                     websocketpp::server<websocketpp::config::asio>  *ws_server,
                                     std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections)
  : ws_server(ws_server), connections(connections)
{
  // set testing mode
  switch (test_mode) {
  case test_device::Mode::Headless:
    mode = { .headless = true, .visual = false, .interactive = false };
    break;
  case test_device::Mode::Visual:
    mode = { .headless = false, .visual = true, .interactive = false };
    break;
  case test_device::Mode::Interactive:
    mode = { .headless = false, .visual = true, .interactive = true };
    break;
  }
}

void BrowserMidiDevice::connect_in(std::string port_name) {
  name = port_name;
  is_input_device = true;
}
void BrowserMidiDevice::connect_out(std::string port_name) {
  name = port_name;
  is_output_device = true;
}

midi_ports_t BrowserMidiDevice::get_port_names() {
  return {};
}
  
void BrowserMidiDevice::listen() {}
void BrowserMidiDevice::emit(types::step::event_t event) {
  if (mode.visual) {
    using json = nlohmann::json;
    json j;
    j["type"] = "midi_event";
    j["event"] = event.data;
    send(j);
  }

  if (is_recording)
    recording_results.insert(event);
}

void BrowserMidiDevice::send(nlohmann::json j) {
  for (auto it : *connections) {
    ws_server->send(it, j.dump(), websocketpp::frame::opcode::value::text);
  }
}


void BrowserMidiDevice::start_recording() {
  is_recording = true;
  recording_results.clear();
}

std::set<types::step::event_t> BrowserMidiDevice::stop_recording() {
  is_recording = false;
  return recording_results;
}


// ----------- BrowserMidiDeviceFactory --------------------

BrowserMidiDeviceFactory::BrowserMidiDeviceFactory(test_device::Mode mode,
                                                   websocketpp::server<websocketpp::config::asio>  *ws_server,
                                                   std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections)
  : mode(mode), ws_server(ws_server), connections(connections) {}

std::map<std::string, std::shared_ptr<BrowserMidiDevice> > BrowserMidiDeviceFactory::get_input_devices() {
  if (input_devices.empty()) {
    for (auto device : devices) {
      if (device->is_input_device)
        input_devices[device->name] = device;
    }
  }

  return input_devices;
}
std::map<std::string, std::shared_ptr<BrowserMidiDevice> > BrowserMidiDeviceFactory::get_output_devices() {
  if (output_devices.empty()) {
    for (auto device : devices) {
      if (device->is_output_device)
        output_devices[device->name] = device;
    }
  }

  return output_devices;
}

std::shared_ptr<MidiDevice> BrowserMidiDeviceFactory::make() {
  auto device = std::make_shared<BrowserMidiDevice>(BrowserMidiDevice(mode, ws_server, connections));
  devices.push_back(device);
  return device;
}

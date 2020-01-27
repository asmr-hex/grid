#include "doubles/device/midi.hpp"


void BrowserMidiDevice::connect_in(std::string port_name) {};
void BrowserMidiDevice::connect_out(std::string port_name) {};

midi_ports_t BrowserMidiDevice::get_port_names() {
  return {};
};
  
void BrowserMidiDevice::listen() {};
void BrowserMidiDevice::emit(midi_event_t) {};

void BrowserMidiDevice::connect_to_browser(websocketpp::server<websocketpp::config::asio>  *ws_server_,
                                           std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections_) {
  ws_server = ws_server_;
  connections = connections_;
};

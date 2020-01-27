/**
 * @file   test/doubles/device/midi.hpp
 * @brief  Midi Device Test Double For Integration Tests
 * @author coco
 * @date   2020-01-25
 *************************************************/


#ifndef TEST_DOUBLE_DEVICE_MIDI_H
#define TEST_DOUBLE_DEVICE_MIDI_H

#include <set>
#include <memory>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include "anemone/io.hpp"


class BrowserMidiDevice : public MidiDevice {
public:
  virtual void connect_in(std::string port_name) override;
  virtual void connect_out(std::string port_name) override;

  virtual midi_ports_t get_port_names() override;
  
  virtual void listen() override;
  virtual void emit(midi_event_t) override;

  void connect_to_browser(websocketpp::server<websocketpp::config::asio>  *ws_server,
                          std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections);

private:
  websocketpp::server<websocketpp::config::asio>  *ws_server;
  std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections;
};

#endif

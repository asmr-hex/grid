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

#include "doubles/device/mode.hpp"

#include "anemone/io.hpp"


class BrowserMidiDevice : public MidiDevice {
public:
  BrowserMidiDevice(test_device::Mode mode,
                    websocketpp::server<websocketpp::config::asio>  *ws_server,
                    std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections);
  virtual void connect_in(std::string port_name) override;
  virtual void connect_out(std::string port_name) override;

  virtual midi_ports_t get_port_names() override;
  
  virtual void listen() override;
  virtual void emit(midi_event_t) override;

  void start_recording();
  std::set<midi_event_t> stop_recording();

  std::string name;
  bool is_output_device = false;
  bool is_input_device = false;
private:
  struct {
    bool headless;
    bool visual;
    bool interactive;
  } mode;
  
  bool is_recording;
  std::set<midi_event_t> recording_results;

  websocketpp::server<websocketpp::config::asio>  *ws_server;
  std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections;
};


class BrowserMidiDeviceFactory : public MidiDeviceFactory {
public:
  BrowserMidiDeviceFactory(test_device::Mode,
                           websocketpp::server<websocketpp::config::asio>*,
                           std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> >*);

  virtual std::shared_ptr<MidiDevice> make() override;

  std::map<std::string, std::shared_ptr<BrowserMidiDevice> > get_input_devices();
  std::map<std::string, std::shared_ptr<BrowserMidiDevice> > get_output_devices();
private:
  test_device::Mode mode;
  websocketpp::server<websocketpp::config::asio>  *ws_server;
  std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > *connections;

  std::vector<std::shared_ptr<BrowserMidiDevice> > devices;
  std::map<std::string, std::shared_ptr<BrowserMidiDevice> > input_devices;
  std::map<std::string, std::shared_ptr<BrowserMidiDevice> > output_devices;
};


#endif

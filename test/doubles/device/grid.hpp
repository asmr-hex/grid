#ifndef TEST_DOUBLE_DEVICE_GRID_H
#define TEST_DOUBLE_DEVICE_GRID_H

// use standalone asio header only library rather than boost.asio.
// see https://think-async.com/Asio/AsioAndBoostAsio.html for details.
#define ASIO_STANDALONE

#include <string>
#include <thread>
#include <httplib.h>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "anemone/io.hpp"
#include "anemone/util/concurrent_queue.hpp"


class BrowserGridDevice : public GridDevice {
public:
  BrowserGridDevice(bool interactive, std::shared_ptr< Queue<bool> > ready)
    : interactive(interactive), ready(ready)
  {
    if (interactive) {
      // set html dir to serve over http
      auto ret = svr.set_base_dir("test/doubles/device/html");
      if (!ret) {
        spdlog::error("the html base dir doesn't exist!");
      }

      // turn off console logging for websocket server
      ws_server.clear_access_channels(websocketpp::log::alevel::frame_header | websocketpp::log::alevel::frame_payload);
      ws_server.clear_access_channels(websocketpp::log::alevel::all);
    }
  };

  virtual void connect(std::string addr) override {
    if (interactive) {
      std::thread t([this, addr] {
                      svr.listen("localhost", std::stoi(addr));
                    });
      t.detach();
      
      // set websocket callback handler
      ws_server.set_message_handler([this] (websocketpp::connection_hdl hndl,
                                            websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
                                      handle_ws_msg(hndl, msg);
                                    });
      // set on_open and on_close handlers
      ws_server.set_open_handler([this] (websocketpp::connection_hdl hndl) {
                                   on_open(hndl);
                                 });
      ws_server.set_close_handler([this] (websocketpp::connection_hdl hndl) {
                                   on_close(hndl);
                                 });

      std::thread tt([this] {
                       // start websocket listener
                       ws_server.init_asio();
                       ws_server.listen(2718);
                       ws_server.start_accept();
                       ws_server.run();                       
                     });
      tt.detach();
    } else {
      ready->push(true);
    }
  };
  virtual void listen() override {};

  virtual void turn_off(grid_coordinates_t c) override {
    using json = nlohmann::json;
    json j;
    j["type"] = "led_event";
    j["action"] = "off";
    j["x"] = c.x;
    j["y"] = c.y;
    send(j);
  };
  virtual void turn_on(grid_coordinates_t c) override {
    using json = nlohmann::json;
    json j;
    j["type"] = "led_event";
    j["action"] = "on";
    j["x"] = c.x;
    j["y"] = c.y;
    send(j);
  };
  virtual void set(grid_coordinates_t c, unsigned int intensity) override {
    using json = nlohmann::json;
    json j;
    j["type"] = "led_event";
    j["action"] = "set";
    j["x"] = c.x;
    j["y"] = c.y;
    j["intensity"] = intensity;
    send(j);
  };

  void toggle(grid_coordinates_t c) {
    // toggle a button (press or unpress)
  };

  void describe(std::string description) {
    // send description to browser
  }
  
private:
  bool interactive;
  httplib::Server svr;
  websocketpp::server<websocketpp::config::asio> ws_server;
  std::shared_ptr< Queue<bool> > ready;
  std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > connections;

  void send(nlohmann::json j) {
    for (auto it : connections) {
      ws_server.send(it, j.dump(), websocketpp::frame::opcode::value::text);
    }
  }
  
  void on_open(websocketpp::connection_hdl hdl) {
    connections.insert(hdl);
  }

  void on_close(websocketpp::connection_hdl hdl) {
    connections.erase(hdl);
  }
  
  void handle_ws_msg(websocketpp::connection_hdl hndl,
                     websocketpp::server<websocketpp::config::asio>::message_ptr msg)
  {
    using json = nlohmann::json;

    auto json_msg = json::parse(msg->get_payload());

    if (json_msg["type"].get<std::string>() == "connected") {
      // send some configuration info
      json j;
      j["type"] = "configuration";
      if (interactive) {
        j["mode"] = "interactive"; 
      }else {
        j["mode"] = "visual";
      }
      ws_server.send(hndl, j.dump(), msg->get_opcode());
    }

    if (json_msg["type"].get<std::string>() == "ready") {
      spdlog::info("starting tests...");
      ready->push(true);
    }
    
  }
};

#endif

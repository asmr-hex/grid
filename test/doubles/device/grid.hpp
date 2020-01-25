#ifndef TEST_DOUBLE_DEVICE_GRID_H
#define TEST_DOUBLE_DEVICE_GRID_H

#include <string>
#include <thread>
#include <httplib.h>
#include <websocketpp/asio_no_tls.hpp>
// #include <websocketpp/config/minimal_server.hpp>
#include <websocketpp/server.hpp>
#include <spdlog/spdlog.h>

#include "anemone/io.hpp"
#include "anemone/util/concurrent_queue.hpp"

typedef websocketpp::server<websocketpp::config::minimal_server> ws_server;

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

      // set websocket callback handler
    }
  };

  virtual void connect(std::string addr) override {
    if (interactive) {
      std::thread t([this, addr] {
                      spdlog::info(addr);
                      svr.listen("localhost", std::stoi(addr));
                    });
      t.detach();

      // start websocket listener
      // ws_server.init_asio();
      ws_server.listen(2718);
      ws_server.start_accept();
      ws_server.run();
    } else {
      ready->push(true);
    }
  };
  virtual void listen() override {};

  virtual void turn_off(grid_coordinates_t c) override {};
  virtual void turn_on(grid_coordinates_t c) override {};
  virtual void set(grid_coordinates_t c, unsigned int intensity) override {};

private:
  bool interactive;
  httplib::Server svr;
  websocketpp::server<websocketpp::config::minimal_server> ws_server;
  std::shared_ptr< Queue<bool> > ready;
};

#endif

#ifndef TEST_MOCKS_DEVICE_GRID_H
#define TEST_MOCKS_DEVICE_GRID_H

#include <utility>
#include <vector>

#include "anemone/io/grid/device/grid.hpp"


class MockGridDevice : public GridDevice {
public:
  virtual void connect(std::string addr) override {};
  virtual void listen() override {};

  virtual void turn_off(grid_coordinates_t c) override {
    leds_turned_off.push_back(c);
  };
  virtual void turn_on(grid_coordinates_t c) override {
    leds_turned_on.push_back(c);
  };
  virtual void set(grid_coordinates_t c, unsigned int intensity) override {
    leds_set.push_back({c, intensity});
  };

  void emit(grid_device_event_t event) {
    broadcast(event);
  };

  std::vector<grid_coordinates_t> leds_turned_off;
  std::vector<grid_coordinates_t> leds_turned_on;
  std::vector< std::pair<grid_coordinates_t, unsigned int> > leds_set;
};

#endif

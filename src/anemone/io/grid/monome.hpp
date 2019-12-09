#ifndef IO_GRID_MONOME_H
#define IO_GRID_MONOME_H

#include <vector>
#include <iostream>

#include <monome.h>

#include "grid.hpp"
#include "observer.hpp"


class Monome : public Grid {
public:
  void connect(std::string addr = "/dev/tty.usbserial-m1000843") {
    // TODO (coco|5.11.2019) parameterize monome serial port location.
    // rpi : /dev/ttyUSB0
    // osx : /dev/tty.usbserial-m1000843
    if( !(monome = monome_open(addr)) ) {
      std::cout << "Could not connect to monome grid!\n";
      exit( EXIT_FAILURE );
    }
    std::cout << "CONNECTED TO MONOME!\n";
  };

  void register_handler(GridObserver *observer) {
    observers.push_back(observer);
  };

  void listen() {
    monome_register_handler(monome, MONOME_BUTTON_DOWN, this->callback_wrapper, this);
    monome_register_handler(monome, MONOME_BUTTON_UP, this->callback_wrapper, this);

    monome_event_loop(monome);
  };

  void turn_off(mapping_coordinates_t c) {
    monome_led_off(monome, c.x, c.y);
  };

  void turn_on(mapping_coordinates_t c) {
    monome_led_on(monome, c.x, c.y);
  };

  void set(mapping_coordinates_t c, unsigned int intensity) {
    monome_led_level_set(monome, c.x, c.y, intensity);
  };

private:
  monome_t *monome;
  std::vector<GridObserver *> observers;

  static void callback_wrapper(const monome_event_t *m_event, void *user_data) {
    Monome *this_monome = (Monome *)user_data;

    grid_event_t event = monome_event_to_grid_event(m_event);
    
    for (GridObserver *observer : this_monome->observers) {
      if (observer->is_relevant(event))
        observer->handle(event);
    }
  };

  static grid_event_t monome_event_to_grid_event(const momnome_event_t *m_event) {
    grid_event_type_t g_event_type;

    switch (m_event->event_type) {
    case MONOME_BUTTON_DOWN:
      g_event_type = BUTTON_DOWN;
      break;
    case MONOME_BUTTON_UP:
      g_event_type = BUTTON_UP;
      break;
    };

    return { .type = g_event_type, .x = m_event->grid.x, .y = m_event->grid.y };
  };
  
};

#endif

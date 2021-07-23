#include <spdlog/spdlog.h>

#include "anemone/io/grid/device/monome.hpp"


void Monome::connect(std::string addr = "/dev/ttyUSB0") {
  // rpi : /dev/ttyUSB0
  // osx : /dev/tty.usbserial-m1000843
  if( !(monome = monome_open(addr.c_str())) ) {
    spdlog::error("could not connect to monome grid");
    exit( EXIT_FAILURE );
  }
  spdlog::info("  connected -> monome");
}

rxcpp::observable<grid_device_event_t> Monome::listen() {
  monome_register_handler(monome,
                          MONOME_BUTTON_DOWN,
                          [] (const monome_event_t *m_event, void *user_data) {                            
                            Monome *this_monome = static_cast<Monome *>(user_data);
                            auto subscriber = this_monome->get_subscriber();
                            subscriber.on_next(grid_device_event_t{{ .x = m_event->grid.x,
                                                                     .y = m_event->grid.y },
                                                                   .type = GridDeviceEvent::ButtonDown,
                              });
                          }, this);
  monome_register_handler(monome,
                          MONOME_BUTTON_UP,
                          [] (const monome_event_t *m_event, void *user_data) {                            
                            Monome *this_monome = static_cast<Monome *>(user_data);
                            auto subscriber = this_monome->get_subscriber();
                            subscriber.on_next(grid_device_event_t{{ .x = m_event->grid.x,
                                                                     .y = m_event->grid.y },
                                                                   .type = GridDeviceEvent::ButtonUp,
                              });
                          }, this);


  std::thread t([this] {
                  monome_event_loop(monome);
                });
  t.detach();

  return get_observable();
}


void Monome::turn_off(grid_coordinates_t c) {
  monome_led_off(monome, c.x, c.y);
}


void Monome::turn_on(grid_coordinates_t c) {
  monome_led_on(monome, c.x, c.y);
}


void Monome::set(grid_coordinates_t c, unsigned int intensity) {
  monome_led_level_set(monome, c.x, c.y, intensity);
}

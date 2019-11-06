#ifndef SEQUENCER_UTILS_H
#define SEQUENCER_UTILS_H

#include "types.hpp"


step_event_t turn_led_off(unsigned int x, unsigned int y) {
  step_event_t event = {
                        .protocol = OSC,
                        .id = (unsigned short)(0x0100 | ( x << 4 ) | y ),
  };

  return event;
};

step_event_t turn_led_on(unsigned int x, unsigned int y) {
  step_event_t event = {
                        .protocol = OSC,
                        .id = (unsigned short)(0x0000 | ( x << 4 ) | y),
  };

  return event;
};

#endif

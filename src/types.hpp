#ifndef TYPES_H
#define TYPES_H

#include <map>
#include <vector>

// TODO (coco|1.11.19) eventually support NRPN commands (particularly for ER-1)

/*
  unique identifier of an event.

  for midi events:
    0x<status byte><data 1 byte>
  where,
    "status byte" is the command + channel
    "data 1 byte" is a 0-127 value (such as pitch)

  for osc events:
    0x<command byte><location byte>
  where,
    "command byte" is on (0x00), off (0x01), or brightness (0x02)
    "location byte" is the x and y coordinates, e.g. 0x<x><y>, on the monome
 */
typedef unsigned short event_uid_t; 

/*
  protocol to use for this event.

  midi events will be routed through the midi out while osc events will
  be routed to the monome.
 */
enum protocol_t { MIDI, OSC };

/*
  osc command type for monome leds.
 */
enum osc_command_t {
                    led_on         = 0x00,
                    led_off        = 0x01,
                    led_brightness = 0x02,
};

/*
  playback status type.
 */
enum playback_t { playing, stopped, paused, muted, solo };

/*
  step event type.

  data is a vector of bytes.
 */
struct step_event_t {
  protocol_t protocol;
  event_uid_t id;
  std::vector<unsigned char> data;
};

/*
  step index type.
 */
typedef unsigned int step_idx_t;

/*
  defines each step in a map, keyed by the step. more space-efficient
  than storing a sparse vector.

  the inner map is keyed by the event unique identifier. this way we have
  constant time look ups for modifying events at certain steps.
*/
typedef std::map<step_idx_t, std::map<event_uid_t, step_event_t> > sequence_t;

#endif

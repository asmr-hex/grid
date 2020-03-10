#ifndef IO_MIDI_EVENT_H
#define IO_MIDI_EVENT_H

#include <vector>
#include <string>


// unique identifier of a midi event.
//
// format:
//   0x<status byte><data 1 byte>
// where,
//   <status byte> is the command + channel
//   <data 1 byte> is a 0-127 value (such as pitch)
typedef unsigned short midi_event_uid_t;


class midi_event_t {
public:
  midi_event_uid_t id;
  std::string source;
  std::string destination;
  double timestamp;
  const std::vector<unsigned char> *data;

  bool operator <(const midi_event_t &rhs) const {
    return id < rhs.id;
  };
  
  midi_event_t(std::string source, double timestamp, const std::vector<unsigned char> *data);

  // TODO (coco|30.12.2019) add more member variables for command type, etc.
  // need to look at the midi spec for this.
};

#endif

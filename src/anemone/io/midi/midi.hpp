#ifndef IO_MIDI_H
#define IO_MIDI_H

#include <string>

#include "anemone/config/config.hpp"
#include "anemone/io/observable.hpp"
#inlcude "anemone/io/midi/device/midi.hpp"


// TODO (coco|12.29.2019) currently, this is abstraction layer doesn't do anything. eventually
// this should be a shim between the raw midi signals and the application controllers which
// translates incoming signal channels to controller specific signals (for the glove for example)
// and translates outbound midi messages. is this a good idea?
class Midi : public Observer<midi_event_t> {
public:
  Midi(std::shared_ptr<Config>, std::shared_ptr<MidiDevice>);

  void connect();

  // TODO (coco|12.29.2019) support emitting midi out msgs.

private:
  std::shared_ptr<MidiDevice> device;
};

#endif

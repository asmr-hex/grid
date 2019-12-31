#ifndef IO_MIDI_H
#define IO_MIDI_H

#include <string>
#include <memory>

#include "anemone/config/config.hpp"
#include "anemone/io/observable.hpp"
#include "anemone/io/observer.hpp"
#include "anemone/io/midi/device/midi.hpp"


// TODO (coco|12.29.2019) currently, this is abstraction layer doesn't do anything. eventually
// this should be a shim between the raw midi signals and the application controllers which
// translates incoming signal channels to controller specific signals (for the glove for example)
// and translates outbound midi messages. is this a good idea?
class Midi : public Observer<midi_event_t>, public Observable<midi_event_t> {
public:
  Midi(std::shared_ptr<Config>, std::shared_ptr<MidiDevice>);

  void connect();

  void emit(midi_event_t);

private:
  std::shared_ptr<MidiDevice> device;
  
  struct {
    std::string in;
    std::string out;
  } ports;

  virtual void handle(const midi_event_t&) override;
};

#endif

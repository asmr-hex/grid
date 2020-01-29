/**
 * @file   io/midi/midi.hpp
 * @brief  Midi Router Class
 * @author coco
 * @date   2020-01-25
 *************************************************/


#ifndef ANEMONE_IO_MIDI_H
#define ANEMONE_IO_MIDI_H

#include <string>
#include <vector>
#include <memory>

#include "anemone/types.hpp"

#include "anemone/config/config.hpp"
#include "anemone/io/observable.hpp"
#include "anemone/io/observer.hpp"
#include "anemone/io/midi/device/midi.hpp"
#include "anemone/io/midi/device/factory.hpp"


// TODO (coco|12.29.2019) currently, this is abstraction layer doesn't do anything. eventually
// this should be a shim between the raw midi signals and the application controllers which
// translates incoming signal channels to controller specific signals (for the glove for example)
// and translates outbound midi messages. is this a good idea?
class Midi : public Observer<midi_event_t>, public Observable<midi_event_t> {
public:
  Midi(std::shared_ptr<Config>, std::shared_ptr<MidiDeviceFactory>);

  void connect();
  void emit(types::step::event_t);
  void emit(std::vector<types::step::event_t>&);

  std::map<std::string, std::shared_ptr<MidiDevice> > input_devices;
  std::map<std::string, std::shared_ptr<MidiDevice> > output_devices;
private:
  std::shared_ptr<MidiDeviceFactory> device_factory;

  std::vector<std::string> input_device_names;
  std::vector<std::string> output_device_names;

  void connect_with_retry(std::string device_name, bool is_input_connection);
  
  virtual void handle(const midi_event_t&) override;
};

#endif

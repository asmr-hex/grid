/**
 * @file   types/instruments/instrument.hpp
 * @brief  Instrument State Base Class Type
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef TYPES_INSTRUMENTS_INSTRUMENT_H
#define TYPES_INSTRUMENTS_INSTRUMENT_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/config.hpp"

#include "anemone/types/instrument/names.hpp"
#include "anemone/types/instrument/parameter/parameter.hpp"
#include "anemone/types/instrument/pad/pad.hpp"
#include "anemone/types/instrument/keyboard/keyboard.hpp"
#include "anemone/types/instrument/step/event.hpp"
#include "anemone/types/instrument/part/part.hpp"
#include "anemone/types/instrument/sequence/sequence.hpp"
#include "anemone/types/io/grid/layout/layout.hpp"
#include "anemone/types/io/midi/event.hpp"


class Instrument {
public:
  Instrument(InstrumentName,
             std::shared_ptr<Config>,
             std::shared_ptr<::Layout>,
             std::vector<std::shared_ptr<Part> >,
             sequence_layer_t);

  InstrumentName name;

  /// @brief pointer to the layout for this instrument
  std::shared_ptr<Layout> layout;

  struct status_t {
    struct part_t {
      rx::behavior<std::shared_ptr<Part> > in_playback;
      rx::behavior<std::shared_ptr<Part> > under_edit;
    };
    struct bank_t {
      rx::behavior<bank_idx_t> in_playback;
      rx::behavior<bank_idx_t> under_edit;
    };

    part_t part;
    bank_t bank;
    rx::behavior<bool> is_playing;
    rx::behavior<bool> stop_on_next_measure;
  };

  status_t status;
  std::vector<std::shared_ptr<Part> > parts;

  /// @brief instrument global parameters
  std::vector< std::shared_ptr<Parameter> > parameters;

  /// @brief pads
  std::vector< std::shared_ptr<Pad> > pads;

  /// @brief midi mapping
  struct {
    midi_channel_t channel = 1;
    std::map<midi_note_number_t, std::shared_ptr<Pad> > note_to_pad;
  } midi_map;
  
  /// @brief instrument keyboard
  std::shared_ptr<Keyboard> keyboard;

  /// @brief an abservable stream of midi events in playback.
  ///
  /// @details this stream is useful for when a specific instrument class
  /// derived from the Instrument class needs to know when/what notes are being
  /// played back by the step sequencer. For example, if there is a drum machine
  /// instrument plugin which needs to turn on/off the leds of specific drum pads
  /// it can subscribe to this stream and filter on specific notes.
  /// events are added to this stream within the step controller when step events are
  /// emitted by the sequencer.
  rx::behavior<midi_event_t> playback_midi_events = rx::behavior<midi_event_t>({});
  
  /// @brief the last midi notes played.
  ///
  /// @description this is used as the default note for when new steps are added by hand.
  rx::behavior<sequence_layer_t> last_midi_notes_played;

  void update_last_midi_notes_played(midi_event_t);

  /// @brief wrapper over Instrument constructor to initialize parts.
  static Instrument create(InstrumentName, std::shared_ptr<Config>);

  /// @brief instrument layout class.
  class Layout : public ::Layout {
  public:
    std::shared_ptr<Config> config;
    
    Layout(InstrumentName, std::shared_ptr<Config>);

    virtual void register_sections() override;
  };
};

#endif

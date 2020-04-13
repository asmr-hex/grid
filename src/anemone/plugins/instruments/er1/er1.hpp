/**
 * @file   plugins/instruments/er1/er1.hpp
 * @brief  ER1 Instrument Plugin Class
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_H
#define PLUGINS_INSTRUMENTS_ER1_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/plugins/plugin.hpp"

#include "anemone/ui/layout_ui.hpp"
#include "anemone/controllers/controller.hpp"

#include "anemone/plugins/instruments/er1/layout/layout.hpp"
#include "anemone/plugins/instruments/er1/controllers/controller.hpp"
#include "anemone/plugins/instruments/er1/ui/ui.hpp"


namespace ER1 {
  class ER1 : public Plugin, public Instrument, public std::enable_shared_from_this<ER1> {
  public:

    /// @brief ER1::Parameter is a controllable instrument parameter.
    /// TODO put this somewhere else!
    ///
    /// @details a Parameter is a variable instrument parameter. a Parameter can be one of
    /// two value types: (1) Discrete (2) Continuous. a Parameter can be Mappable or not,
    /// meaning it can be ....hmm should ever parameter be mappable?
    class Parameter {
    public:
      enum class ValueType { Discrete, Continuous };

      ValueType type = ValueType::Continuous;
      std::string name;

      struct {
        rx::behavior<unsigned int> current  = rx::behavior<unsigned int>(80);
        rx::behavior<unsigned int> playback = rx::behavior<unsigned int>(0);
      } value;

      struct {
        rx::behavior<bool> mapping_in_progress = rx::behavior<bool>(false);
        std::string  source;
        unsigned int status_byte;
        unsigned int control;
      } midi_map;

      /// @brief this function is provided for each instrument and is implemented according
      /// to the midi specification of the instrument to which this parameter belongs.
      ///
      /// @details since the midi-controllable parameters on an instrument can be controlled
      /// by any type of midi messages, inncludinng Non-Registered PArameter Numbers (NRPN),
      /// this function provides flexibility to map a scalar parameter value to the appropriate
      /// output midi messages needed to control the parameter on the instrument (according to
      /// the instrument's specific midi specification).
      ///
      std::function<std::vector<midi_event_t>(unsigned int)> convert_value_to_midi_events;

      /// @brief this function is used to translate an inbound midi event to a current value.
      /// it essentially just extracts the appropriate value from the inbound event. it can be
      /// thought of as the inverse to the provided `convert_value_to_midi_event` function in a
      /// way.
      ///
      /// @details since we want to be able to map a wide range of different inbound midi message
      /// types (cc, pitch wheel, etc.) to the parameter value, we will need to provide a way to
      /// extract the appropriate value from these different types of input messages.
      ///
      /// NOTE: for now we will be limited to CC messages.
      std::function<unsigned int(midi_event_t)> convert_midi_event_to_value;

      /// @brief determines whether a midi event maps to this parameter.
      bool is_mapped_to(midi_event_t);

      /// @brief takes an inbound midi event and extracts the new parameter value from it
      /// in order to update the current parameter value. this method is the inverse of the
      /// `get_midi_output` method which maps the current value to its appropriate output
      /// midi message(s).
      void set_current_value(midi_event_t);

      /// @brief generates output midi events for the current parameter value.
      ///
      /// @remark since the midi output could be NRPN messages, which consist of
      /// three consecutive midi events, we are returning a vector of midi events.
      std::vector<midi_event_t> get_midi_output();

      Parameter(std::string name);
    };
    
    /// @brief ER1::Pad is a triggerable sound on the drum machine, it is equivalent
    /// to a drum part.
    /// TODO this could actually maybe be a nested class on instrument.....?
    /// TODO should this be called Layer instead?
    class Pad {
    public:
      /// @brief is the pad selected?
      rx::behavior<bool> is_selected = rx::behavior<bool>(false);

      /// @brief is the pad playing?
      rx::behavior<bool> is_playing = rx::behavior<bool>(false);

      struct {
        /// @brief the output channel for midi messages.
        midi_channel_t channel;
      
        /// @brief scientific pitch notation representation of midi note which this pad triggers.
        midi_spn_t spn;

        /// @brief midi note number which this pad triggers.
        midi_note_number_t note_number;

        /// @brief the note on data to be emitted when this pad is triggered.
        midi_data_t note_on_data;

        /// @brief the note off data to be emitted when this pad is untriggered.
        midi_data_t note_off_data;
        
      } midi;

      /// @brief pad specific parameters.
      std::vector< std::shared_ptr<Parameter> > parameters;
      
      Pad(midi_spn_t, midi_channel_t);
    };

    /// @brief midi mapping
    struct {
      midi_channel_t channel = 10;
      std::map<midi_note_number_t, std::shared_ptr<Pad> > note_to_pad;
    } midi_map;

    struct {
      /// @brief oscillator pads
      std::vector< std::shared_ptr<Pad> >
      oscillators = { {std::make_shared<Pad>("c2", 10)},
                      {std::make_shared<Pad>("c#2", 10)},
                      {std::make_shared<Pad>("d2", 10)},
                      {std::make_shared<Pad>("d#2", 10)},                                   
      };

      /// @brief cymbal pads
      std::vector< std::shared_ptr<Pad> >
      cymbals = { {std::make_shared<Pad>("e2", 10)},
                  {std::make_shared<Pad>("f2", 10)},
                  {std::make_shared<Pad>("f#2", 10)},
                  {std::make_shared<Pad>("g2", 10)},
      };
      
      /// @brief audio in pads
      std::vector< std::shared_ptr<Pad> >
      audio_ins = { {std::make_shared<Pad>("g#2", 10)},
                    {std::make_shared<Pad>("a2", 10)},
      };

      /// @brief which pad is the currently selected (to show its parameters)
      rx::behavior< std::shared_ptr<Pad> > selected = rx::behavior< std::shared_ptr<Pad> >(nullptr);
      
    } pads;

    /// @breif global parameters
    std::vector< std::shared_ptr<Parameter> >
    parameters = { {std::make_shared<Parameter>("delay depth")},
                   {std::make_shared<Parameter>("delay time")}
    };

    ER1(std::shared_ptr<Config>);

    virtual std::shared_ptr<Layout>
    get_layout() override;

    virtual std::shared_ptr<StateController>
    make_controller(std::shared_ptr<IO>, std::shared_ptr<State>) override;

    virtual std::shared_ptr<LayoutUI>
    make_ui(std::shared_ptr<IO>, std::shared_ptr<State>) override;
  }; 
};

#endif

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

    /// @brief ER1::Pad is a triggerable sound on the drum machine, it is equivalent
    /// to a drum part.
    /// TODO this could actually maybe be a nested class on instrument.....?
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
    } pads;

      
      
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

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
    /// @brief midi mapping
    struct {
      unsigned int channel = 10;
      struct {
        midi_spn_t osc1 = "c2";
        midi_spn_t osc2 = "c#2";
        midi_spn_t osc3 = "d2";
        midi_spn_t osc4 = "d#2";
      } notes;
    } midi_map;


    /// @brief keep track of the playing pads
    struct {
      rx::behavior<bool> osc1 = rx::behavior<bool>(false);
      rx::behavior<bool> osc2 = rx::behavior<bool>(false);
      rx::behavior<bool> osc3 = rx::behavior<bool>(false);
      rx::behavior<bool> osc4 = rx::behavior<bool>(false);
    } pad_is_playing;
    
    
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

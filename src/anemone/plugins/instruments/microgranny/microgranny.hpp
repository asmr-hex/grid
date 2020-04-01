/**
 * @file   plugins/instruments/microgranny/microgranny.hpp
 * @brief  MicroGranny Instrument Plugin Class
 * @author coco
 * @date   2020-03-28
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_MICROGRANNY_H
#define PLUGINS_INSTRUMENTS_MICROGRANNY_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/plugins/plugin.hpp"

#include "anemone/ui/layout_ui.hpp"
#include "anemone/controllers/controller.hpp"

#include "anemone/plugins/instruments/microgranny/layout/layout.hpp"
#include "anemone/plugins/instruments/microgranny/controllers/controller.hpp"
#include "anemone/plugins/instruments/microgranny/ui/ui.hpp"


namespace MicroGranny {
  class MicroGranny : public Plugin, public Instrument, public std::enable_shared_from_this<MicroGranny> {
  public:
    /// @brief midi mapping
    struct {
      unsigned int channel = 6;
      std::vector<std::string> notes = { { "c-1"  },
                                         { "c#-1" },
                                         { "d-1"  },
                                         { "d#-1" },
                                         { "e-1"  },
                                         { "f-1"  }
      };
    } midi_map;


    /// @brief keep track of the playing pads
    std::vector<rx::behavior<bool> > pad_is_playing = { { rx::behavior<bool>(false) },
                                                        { rx::behavior<bool>(false) },
                                                        { rx::behavior<bool>(false) },
                                                        { rx::behavior<bool>(false) },
                                                        { rx::behavior<bool>(false) },
                                                        { rx::behavior<bool>(false) }
    };
    
    MicroGranny(std::shared_ptr<Config>);

    virtual std::shared_ptr<Layout>
    get_layout() override;

    virtual std::shared_ptr<StateController>
    make_controller(std::shared_ptr<IO>, std::shared_ptr<State>) override;

    virtual std::shared_ptr<LayoutUI>
    make_ui(std::shared_ptr<IO>, std::shared_ptr<State>) override;
  }; 
};

#endif

/**
 * @file   plugins/instruments/er1/ui.hpp
 * @brief  ER1 UI
 * @author coco
 * @date   2020-03-21
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_UI_H
#define PLUGINS_INSTRUMENTS_ER1_UI_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/config.hpp"
#include "anemone/plugins.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/plugins/instruments/er1/ui/osc1.hpp"


namespace ER1 {
  class UI : public LayoutUI {
  public:
    UI(std::shared_ptr<Config>,
          std::shared_ptr<IO>,
          std::shared_ptr<State>);

  private:
    std::shared_ptr<Config> config;
    std::shared_ptr<IO> io;
    std::shared_ptr<State> state;
    
    std::unique_ptr<OSC1_UI> osc1;
  
  };  
};

#endif
/**
 * @file   plugins/instruments/microgranny/ui.hpp
 * @brief  MicroGranny UI
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

#include "anemone/plugins/instruments/microgranny/ui/samples.hpp"


namespace MicroGranny {

  // forward declaration
  class MicroGranny;
  
  class UI : public LayoutUI {
  public:
    UI(std::shared_ptr<Config>,
       std::shared_ptr<IO>,
       std::shared_ptr<State>,
       std::shared_ptr<MicroGranny>);

  private:
    std::shared_ptr<Config> config;
    std::shared_ptr<IO> io;
    std::shared_ptr<State> state;
    
    std::unique_ptr<SamplesUI> samples;
  
  };  
};

#endif

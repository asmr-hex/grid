/**
 * @file   plugins/instruments/microgranny/layout.hpp
 * @brief  MicroGranny Layout Class
 * @author coco
 * @date   2020-03-21
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_MICROGRANNY_LAYOUT_H
#define PLUGINS_INSTRUMENTS_MICROGRANNY_LAYOUT_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"


namespace MicroGranny {
  class MicroGrannyLayout : public Layout {
  public:
    std::shared_ptr<GridSection> samples;
    std::shared_ptr<GridSection> presets;
    std::shared_ptr<GridSection> banks;
  
    MicroGrannyLayout(std::shared_ptr<Config> config);

    virtual void register_sections() override;
  };

};

#endif

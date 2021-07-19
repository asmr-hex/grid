/**
 * @file   plugins/instruments/er1/layout.hpp
 * @brief  ER1 Layout Class
 * @author coco
 * @date   2020-03-21
 *************************************************/


#ifndef PLUGINS_INSTRUMENTS_ER1_LAYOUT_H
#define PLUGINS_INSTRUMENTS_ER1_LAYOUT_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/types.hpp"


namespace ER1 {
  class ER1Layout : public Layout {
  public:
    std::shared_ptr<GridSection> oscillators;
    std::shared_ptr<GridSection> cymbals;
    std::shared_ptr<GridSection> audio_ins;
    std::shared_ptr<GridSection> delay_depth;
    std::shared_ptr<GridSection> delay_time;
    std::shared_ptr<GridSection> level;
    std::shared_ptr<GridSection> pan;
    std::shared_ptr<GridSection> low_boost;
    std::shared_ptr<GridSection> pitch;
    std::shared_ptr<GridSection> decay;
    std::shared_ptr<GridSection> mod_type;
    std::shared_ptr<GridSection> mod_depth;
    std::shared_ptr<GridSection> mod_speed;
    std::shared_ptr<GridSection> wave_type;
  
    ER1Layout(std::shared_ptr<Config> config);

    virtual void register_sections() override;
  };

};

#endif

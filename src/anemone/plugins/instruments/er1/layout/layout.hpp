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
  
    ER1Layout(std::shared_ptr<Config> config);

    virtual void register_sections() override;
  };

};

#endif

/**
 * @file   state/instruments/er1.hpp
 * @brief  ER1 Instrument Class
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef STATE_INSTRUMENTS_ER1_H
#define STATE_INSTRUMENTS_ER1_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/state/instruments/instrument.hpp"

#include "anemone/state/instruments/er1/layout.hpp"


class ER1 : public Instrument {
public:
  std::shared_ptr<ER1Layout> layout;
  
  ER1(std::shared_ptr<Config>);
};


#endif

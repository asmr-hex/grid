/**
 * @file   state/instruments/er1.hpp
 * @brief  ER1 Instrument Class
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef STATE_INSTRUMENTS_ER1_H
#define STATE_INSTRUMENTS_ER1_H

#include "anemone/config.hpp"
#include "anemone/state/instruments/instrument.hpp"


class ER1 : public Instrument {
public:
  ER1(std::shared_ptr<Config>);
};


#endif

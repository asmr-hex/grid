/**
 * @file   action/play_pause.hpp
 * @brief  Play & Pause Related Actions
 * @author coco
 * @date   2020-01-31
 *************************************************/


#ifndef ANEMONE_ACTION_PLAY_PAUSE_H
#define ANEMONE_ACTION_PLAY_PAUSE_H


#include "anemone/state/instruments/names.hpp"


namespace action {
  
  struct instrument_played_or_paused {
    State::InstrumentName instrument_name;
  };
}

#endif

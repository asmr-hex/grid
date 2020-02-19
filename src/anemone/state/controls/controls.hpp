/**
 * @file   state/controls/controls.hpp
 * @brief  Global Anemone Controls State
 * @author coco
 * @date   2020-02-19
 *************************************************/


#ifndef STATE_CONTROLS_CONTROLS_H
#define STATE_CONTROLS_CONTROLS_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/config.hpp"


class GlobalControls {
public:
  GlobalControls(std::shared_ptr<Config>);

  rx::behavior<bool> shift;
  void set_shift(bool);
};

#endif

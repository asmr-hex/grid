/**
 * @file   state/state.hpp
 * @brief  Aggregate State Class
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef STATE_STATE_H
#define STATE_STATE_H

#include <memory>

#include "anemone/config.hpp"
#include "anemone/state/layout/context.hpp"
#include "anemone/state/controls/controls.hpp"
#include "anemone/state/instruments.hpp"


class State {
public:
  State(std::shared_ptr<Config>);

  std::shared_ptr<LayoutContext> layout;
  std::shared_ptr<GlobalControls> controls;
  std::shared_ptr<Instruments> instruments;
};

#endif

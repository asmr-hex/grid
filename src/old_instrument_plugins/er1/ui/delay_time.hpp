/**
 * @file   plugins/instruments/er1/ui/delay_time.hpp
 * @brief  ER1 Delay Time Paramter UI
 * @author coco
 * @date   2020-04-13
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_UI_DELAY_TIME_PARAM_H
#define PLUGINS_INSTRUMENTS_ER1_UI_DELAY_TIME_PARAM_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"

namespace ER1 {

  // forward declaration
  class ER1;

  class DelayTimeUI : public UIComponent {
  public:
    DelayTimeUI(LayoutName,
           GridSectionName,
           std::shared_ptr<IO>,
           std::shared_ptr<State>,
           std::shared_ptr<ER1>);
  };
};

#endif

/**
 * @file   plugins/instruments/er1/ui/osc.hpp
 * @brief  ER1 OSC UI
 * @author coco
 * @date   2020-03-22
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_UI_OSC_H
#define PLUGINS_INSTRUMENTS_ER1_UI_OSC_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"

namespace ER1 {

  // forward declaration
  class ER1;

  class OSC_UI : public UIComponent {
  public:
    OSC_UI(LayoutName,
           GridSectionName,
           std::shared_ptr<IO>,
           std::shared_ptr<State>,
           std::shared_ptr<ER1>);
  };
};

#endif

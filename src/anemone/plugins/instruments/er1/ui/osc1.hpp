/**
 * @file   plugins/instruments/er1/ui/osc1.hpp
 * @brief  ER1 OSC1 UI
 * @author coco
 * @date   2020-03-22
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_UI_OSC1_H
#define PLUGINS_INSTRUMENTS_ER1_UI_OSC1_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"

namespace ER1 {

  // forward declaration
  class ER1;

  class OSC1_UI : public UIComponent {
  public:
    OSC1_UI(LayoutName,
            GridSectionName,
            std::shared_ptr<IO>,
            std::shared_ptr<State>,
            std::shared_ptr<ER1>);
  };
};

#endif

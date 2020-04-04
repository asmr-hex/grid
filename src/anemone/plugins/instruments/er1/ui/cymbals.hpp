/**
 * @file   plugins/instruments/er1/ui/cymbals.hpp
 * @brief  ER1 Cymbals UI
 * @author coco
 * @date   2020-04-03
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_UI_CYMBALS_H
#define PLUGINS_INSTRUMENTS_ER1_UI_CYMBALS_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"

namespace ER1 {

  // forward declaration
  class ER1;

  class CymbalsUI : public UIComponent {
  public:
    CymbalsUI(LayoutName,
              GridSectionName,
              std::shared_ptr<IO>,
              std::shared_ptr<State>,
              std::shared_ptr<ER1>);
  };
};

#endif

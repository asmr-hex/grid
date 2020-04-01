/**
 * @file   plugins/instruments/microgranny/ui/osc1.hpp
 * @brief  MicroGranny Samples UI
 * @author coco
 * @date   2020-03-28
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_MICROGRANNY_SAMPLES_UI_H
#define PLUGINS_INSTRUMENTS_MICROGRANNY_SAMPLES_UI_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"

namespace MicroGranny {

  // forward declaration
  class MicroGranny;

  class SamplesUI : public UIComponent {
  public:
    SamplesUI(LayoutName,
            GridSectionName,
            std::shared_ptr<IO>,
            std::shared_ptr<State>,
            std::shared_ptr<MicroGranny>);
  };
};

#endif

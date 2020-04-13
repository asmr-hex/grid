/**
 * @file   plugins/instruments/er1/ui/audio_ins.hpp
 * @brief  ER1 Audio Ins UI
 * @author coco
 * @date   2020-04-03
 *************************************************/

#ifndef PLUGINS_INSTRUMENTS_ER1_UI_AUDIO_INS_H
#define PLUGINS_INSTRUMENTS_ER1_UI_AUDIO_INS_H

#include <memory>

#include "anemone/rx.hpp"
#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"

namespace ER1 {

  // forward declaration
  class ER1;

  class AudioInsUI : public UIComponent {
  public:
    AudioInsUI(LayoutName,
              GridSectionName,
              std::shared_ptr<IO>,
              std::shared_ptr<State>,
              std::shared_ptr<ER1>);
  };
};

#endif

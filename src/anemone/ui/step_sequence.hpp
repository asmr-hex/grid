/**
 * @file   ui/step_sequence.hpp
 * @brief  Step Sequence UI
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef UI_STEP_SEQUENCE_H
#define UI_STEP_SEQUENCE_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class StepSequenceUI : public UIComponent {
public:
  StepSequenceUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);

private:
  std::set<page_relative_step_idx_t> rendered_steps;

  /// @brief led levels
  struct {
    unsigned int active = 15;
    unsigned int inactive = 0;
    unsigned int cursor_on_active_step = 6;
    unsigned int cursor = 15;
  } led_level;
};

#endif

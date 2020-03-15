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

  /// @brief previous values
  struct {
    page_idx_t       rendered_page  = 1;
    paged_step_idx_t last_step      = { .page = 1, .step = 31 };
    bool             show_last_step = false;
  } previous;
  
  /// @brief led levels
  struct {
    unsigned int active = 15;
    unsigned int inactive = 0;
    unsigned int cursor_on_active_step = 6;
    unsigned int cursor = 15;

    /// @brief animations
    struct {
      std::shared_ptr<animation::Blink> last_step
      = std::make_shared<animation::Blink>(std::chrono::milliseconds(200), 15, 10, 0.5);
    } animate;
  } led_level;
};

#endif

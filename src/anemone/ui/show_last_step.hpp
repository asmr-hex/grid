/**
 * @file   ui/show_last_step.hpp
 * @brief  Show Last Step UI
 * @author coco
 * @date   2020-03-11
 *************************************************/

#ifndef UI_SHOW_LAST_STEP_H
#define UI_SHOW_LAST_STEP_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class ShowLastStepUI : public UIComponent {
public:
  ShowLastStepUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
private:
  /// @brief previous states
  struct {
    bool show_last_step = false;
    bool follow_cursor  = false;
  } previous;

  /// @brief led levels
  struct {
    unsigned int show_last_step = 15;

    /// @brief led animations
    struct {
      std::shared_ptr<animation::Blink> follow_cursor
      = std::make_shared<animation::Blink>(std::chrono::milliseconds(500), 6, 2, 0.6);
    } animate;
  } led_level;
};

#endif

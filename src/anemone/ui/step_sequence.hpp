/**
 * @file   ui/step_sequence.hpp
 * @brief  Step Sequence UI
 * @author coco
 * @date   2020-02-20
 *************************************************/

#ifndef UI_SHIFT_H
#define UI_SHIFT_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class StepSequenceUI : public UIComponent {
public:
  StepSequenceUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif

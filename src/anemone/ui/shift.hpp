/**
 * @file   ui/shift.hpp
 * @brief  Shift UI
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef UI_SHIFT_H
#define UI_SHIFT_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class ShiftUI : public UIComponent {
public:
  ShiftUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif

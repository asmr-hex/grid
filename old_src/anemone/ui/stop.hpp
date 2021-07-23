/**
 * @file   ui/stop.hpp
 * @brief  Stop UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_STOP_H
#define UI_STOP_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class StopUI : public UIComponent {
public:
  StopUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif

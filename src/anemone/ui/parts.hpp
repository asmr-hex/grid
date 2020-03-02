/**
 * @file   ui/parts.hpp
 * @brief  Parts UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_PARTS_H
#define UI_PARTS_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class PartsUI : public UIComponent {
public:
  PartsUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif

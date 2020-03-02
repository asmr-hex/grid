/**
 * @file   ui/page.hpp
 * @brief  Page UI
 * @author coco
 * @date   2020-03-02
 *************************************************/

#ifndef UI_PAGE_H
#define UI_PAGE_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/component.hpp"


class PageUI : public UIComponent {
public:
  PageUI(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);
};

#endif

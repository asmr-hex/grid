#include "anemone/state/layout/context.hpp"

#include <spdlog/spdlog.h>


LayoutContext::LayoutContext(std::shared_ptr<GridLayouts> layouts)
  : rxcpp::subjects::behavior< std::shared_ptr<Layout> >(layouts->sequencer),
    layouts(layouts)
{
  current_layout = layouts->sequencer;
}


void LayoutContext::use_layout(LayoutName name) {
  current_layout = layouts->layout_by_name[name];

  get_subscriber().on_next(current_layout);
}

std::shared_ptr<GridLayouts> LayoutContext::get_layouts() {
  return layouts;
}

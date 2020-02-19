#include "anemone/state/layout/context.hpp"

#include <spdlog/spdlog.h>


LayoutContext::LayoutContext(layout_initializer_list layouts)
  : rxcpp::subjects::behavior< std::shared_ptr<Layout> >(*layouts.begin())
{
  if (layouts.size() == 0)
    throw std::length_error("no layouts provided");

  for (auto layout : layouts)
    layout_by_name[layout->name] = layout;

  current_layout = *layouts.begin();
}


void LayoutContext::use_layout(LayoutName name) {
  current_layout = layout_by_name[name];

  get_subscriber().on_next(current_layout);
}

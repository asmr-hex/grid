#include "anemone/ui/component.hpp"


UIComponent::UIComponent(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : layout(layout),
    section(section),
    io(io),
    state(state)
{
  section_size = state->layout->get_layouts()->layout_by_name[layout]->section_by_name[section]->size();
}

void UIComponent::turn_on_led(grid_section_index index) {
  io->grid->turn_on
    ({ .layout  = layout,
       .section = section,
       .index   = index,
    });
}

void UIComponent::turn_off_led(grid_section_index index) {
  io->grid->turn_off
    ({ .layout  = layout,
       .section = section,
       .index   = index,
    });
}

void UIComponent::set_led(grid_section_index index, unsigned int intensity) {
  io->grid->set
    ({ .layout  = layout,
       .section = section,
       .index   = index,
    }, intensity);
}

void UIComponent::clear() {
  for (unsigned int i = 0; i < section_size; i++) {
    turn_off_led(i);
  }
}

#include "anemone/ui/component.hpp"


UIComponent::UIComponent(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : layout(layout),
    section(section),
    io(io),
    state(state)
{}

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

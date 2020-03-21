#include <spdlog/spdlog.h>

#include "anemone/ui/component.hpp"


UIComponent::UIComponent(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : layout(layout),
    section(section),
    io(io),
    state(state)
{
  section_size = state->layouts->layout_by_name[layout]->section_by_name[section]->size();
}

void UIComponent::turn_on_led(grid_section_index index) {
  io->grid->turn_on
    ({ .layout  = layout,
       .section = section,
       .index   = index,
    });
}

void UIComponent::turn_on_leds(std::vector<grid_section_index> indices) {
  for (auto index : indices) {
    turn_on_led(index);
  }
}

void UIComponent::turn_off_led(grid_section_index index) {
  // removes animation and sets intensity to 0
  remove_animation(index, 0);
}

void UIComponent::set_led(grid_section_index index, unsigned int intensity) {
  io->grid->set
    ({ .layout  = layout,
       .section = section,
       .index   = index,
    }, intensity);
}

void UIComponent::set_leds(std::vector<grid_section_index> indices, unsigned int intensity) {
  for (auto index : indices) {
    set_led(index, intensity);
  }
}

void UIComponent::add_animation(std::shared_ptr<Animation> animation, grid_section_index index) {
  io->grid->animation->add
    (animation,
     { .layout  = layout,
       .section = section,
       .index   = index,
    });
}

void UIComponent::add_animation(std::map<grid_section_index, std::shared_ptr<Animation> > index_to_animation) {
  std::unordered_map<grid_addr_t, std::shared_ptr<Animation>, grid_addr_hasher> addr_to_animation;
  
  for (auto itr : index_to_animation) {
    addr_to_animation[{.layout = layout, .section = section, .index = itr.first }] = itr.second;
  }
  io->grid->animation->add(addr_to_animation);
}

void UIComponent::remove_animation(grid_section_index index) {
  io->grid->animation->remove
    ({ .layout  = layout,
       .section = section,
       .index   = index,
     });
}

void UIComponent::remove_animation(grid_section_index index, unsigned int intensity) {
  io->grid->animation->remove
    ({ .layout  = layout,
       .section = section,
       .index   = index,
    }, intensity);
}

void UIComponent::remove_animation(std::vector<grid_section_index> indices) {
  std::vector<grid_addr_t> grid_addrs;

  for (auto index : indices) {
    grid_addrs.push_back({ .layout  = layout,
                           .section = section,
                           .index   = index,
      });
  }

  io->grid->animation->remove(grid_addrs);
}

void UIComponent::remove_animation(std::vector<grid_section_index> indices, unsigned int intensity) {
  std::vector<grid_addr_t> grid_addrs;

  for (auto index : indices) {
    grid_addrs.push_back({ .layout  = layout,
                           .section = section,
                           .index   = index,
      });
  }

  io->grid->animation->remove(grid_addrs, intensity);
}

void UIComponent::clear() {
  for (unsigned int i = 0; i < section_size; i++) {
    turn_off_led(i);
  }
}

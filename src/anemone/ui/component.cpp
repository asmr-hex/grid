#include <spdlog/spdlog.h>

#include "anemone/ui/component.hpp"


UIComponent::UIComponent(LayoutName layout, GridSectionName section, std::shared_ptr<IO> io, std::shared_ptr<State> state)
  : layout(layout),
    section(section),
    io(io),
    state(state)
{
  std::vector< std::shared_ptr<Layout> > top_level_layouts;
  for (auto itr : state->layouts->layout_by_name) {
    top_level_layouts.push_back(itr.second);
  }
    
  if ( !get_section_size(top_level_layouts) ) {
    spdlog::error("could not find ui component's corresponding layout/section!");
    exit( EXIT_FAILURE );
  }
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

bool UIComponent::get_section_size(std::vector< std::shared_ptr<Layout> > layouts) {
    // get the size of this section. since this section may not be in a top-level layout
  // (i.e. it may be a sublayout) we must search through the layout tree to find this particular
  // layout/section. depth-first search.
  //
  // NOTE: this will find the *first* matching layout/section which means that if there is a
  // layout/section pair with the *same* names in another branch of the layout tree, it will
  // not take that into account. the upshot of this is that all layout/section pairs in the
  // layout tree *must* be unique.
  //
  // TODO relax this constraint when we eventually refactor everything
  // to be conditioned on the current top-level layout, for example, in the future, there may be many
  // InstrumentA ui's each for a given top-level layout, thus we need to encode that a specific ui is
  // a child ui of a particular top-level layout (*~dreams of the future*~).
  for (auto candidate_layout : layouts) {
    if (candidate_layout->name == layout) {
      // great, does this layout contain the right section?
      try {
        auto matched_section = candidate_layout->section_by_name.at(section);

        // cool! lets set the size.
        section_size = matched_section->size();

        return true;
      } catch (std::out_of_range &error) {
        // dag, this layout doesn't contain that section...
        spdlog::error("section not contained in layout!");
        exit( EXIT_FAILURE );
      }
    } else {
      // hmm okay, maybe the layout is a sublayout of this layout.

      return get_section_size(candidate_layout->sublayouts);
    }
  }

  return false;
}

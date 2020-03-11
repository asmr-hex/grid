/**
 * @file   ui/component.hpp
 * @brief  Base Class for UI Components
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <map>
#include <memory>
#include <vector>
#include <unordered_map>

#include "anemone/types.hpp"
#include "anemone/io.hpp"
#include "anemone/state.hpp"


class UIComponent {
public:
  UIComponent(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);

protected:
  void turn_on_led(grid_section_index index);
  void turn_on_leds(std::vector<grid_section_index> indices);
  void turn_off_led(grid_section_index index);
  void set_led(grid_section_index index, unsigned int intensity);
  void set_leds(std::vector<grid_section_index> indices, unsigned int intensity);


  void add_animation(std::shared_ptr<Animation>, grid_section_index);
  void add_animation(std::map<grid_section_index, std::shared_ptr<Animation> >);
  void remove_animation(grid_section_index);
  void remove_animation(grid_section_index, unsigned int);
  void remove_animation(std::vector<grid_section_index>);
  void remove_animation(std::vector<grid_section_index>, unsigned int);
  
  /// @brief clears the ui section to a tabula rasa
  void clear();

private:
  LayoutName layout;
  GridSectionName section;
  std::shared_ptr<IO> io;
  std::shared_ptr<State> state;
  unsigned int section_size;
};

#endif

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
  void turn_on_led(grid_section_index_t index);
  void turn_on_leds(std::vector<grid_section_index_t> indices);
  void turn_off_led(grid_section_index_t index);
  void set_led(grid_section_index_t index, unsigned int intensity);
  void set_leds(std::vector<grid_section_index_t> indices, unsigned int intensity);


  void add_animation(std::shared_ptr<Animation>, grid_section_index_t);
  void add_animation(std::map<grid_section_index_t, std::shared_ptr<Animation> >);
  void remove_animation(grid_section_index_t);
  void remove_animation(grid_section_index_t, unsigned int);
  void remove_animation(std::vector<grid_section_index_t>);
  void remove_animation(std::vector<grid_section_index_t>, unsigned int);
  
  /// @brief clears the ui section to a tabula rasa
  void clear();

private:
  LayoutName layout;
  GridSectionName section;
  std::shared_ptr<IO> io;
  std::shared_ptr<State> state;
  unsigned int section_size;

  bool get_section_size(std::vector< std::shared_ptr<Layout> >);
};

#endif

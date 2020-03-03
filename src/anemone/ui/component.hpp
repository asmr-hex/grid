/**
 * @file   ui/component.hpp
 * @brief  Base Class for UI Components
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include <memory>

#include "anemone/types.hpp"
#include "anemone/io.hpp"
#include "anemone/state.hpp"


class UIComponent {
public:
  UIComponent(LayoutName, GridSectionName, std::shared_ptr<IO>, std::shared_ptr<State>);

protected:
  void turn_on_led(grid_section_index index);
  void turn_off_led(grid_section_index index);
  void set_led(grid_section_index index, unsigned int intensity);

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

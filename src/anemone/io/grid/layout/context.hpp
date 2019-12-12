#ifndef IO_GRID_LAYOUT_CONTEXT_H
#define IO_GRID_LAYOUT_CONTEXT_H

#include <map>
#include <memory>
#include <initializer_list>

#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/layout/address.hpp"
#include "anemone/io/grid/layout/names.hpp"


typedef std::initializer_list< std::shared_ptr<Layout> > layout_initializer_list;

class LayoutContext {
public:
  LayoutContext(layout_initializer_list layouts);

  void notify(grid_device_event_t&&);
  grid_coordinates_t translate(grid_addr_t&&);
  
private:
  std::shared_ptr<Layout> current_layout;
  std::map< LayoutName, std::shared_ptr<Layout> > layout_by_name;

  void use_layout(LayoutName);

  friend class Layout;
};

#endif

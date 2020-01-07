#ifndef IO_GRID_LAYOUT_CONTEXT_H
#define IO_GRID_LAYOUT_CONTEXT_H

#include <map>
#include <memory>
#include <initializer_list>

#include "anemone/io/grid/device/events.hpp"
#include "anemone/io/grid/device/coordinates.hpp"
#include "anemone/io/grid/layout/layout.hpp"
#include "anemone/io/grid/address.hpp"
#include "anemone/io/grid/layout/names.hpp"


typedef std::initializer_list< std::shared_ptr<Layout> > layout_initializer_list;

class LayoutContext {
public:
  LayoutContext(layout_initializer_list layouts);

  void notify(const grid_device_event_t&);
  grid_coordinates_t translate(const grid_addr_t&);

  LayoutName name() { return current_layout->name; };
  std::map< LayoutName, std::shared_ptr<Layout> > layout_by_name;
private:
  std::shared_ptr<Layout> current_layout;

  void use_layout(LayoutName);

  friend class Layout;
};

#endif

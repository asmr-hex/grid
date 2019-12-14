#ifndef IO_GRID_EVENT_H
#define IO_GRID_EVENT_H

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/address.hpp"


enum class GridEvent { Pressed, Unpressed };

struct grid_event_t : public grid_addr_t {
  GridEvent type;

  static grid_event_t make(LayoutName l, GridSectionName s, unsigned int i, GridEvent t) {
    return {{
             .layout = l,
             .section = s,
             .index = i,
             },
            .type = t
    };
  };
  
  bool operator==(const grid_event_t &m) const {
    return
      type == m.type &&
      layout == m.layout &&
      section == m.section &&
      index == m.index;
  };
};

#endif

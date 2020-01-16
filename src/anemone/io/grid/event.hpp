/**
 * @file   io/grid/event.hpp
 * @brief  IO High-Level Grid Event
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_GRID_EVENT_H
#define IO_GRID_EVENT_H

#include "anemone/io/grid/layout/names.hpp"
#include "anemone/io/grid/address.hpp"


/// @brief Grid event type
enum class GridEvent {
                      /// a grid button was pressed down
                      Pressed,
                      /// a grid button was released
                      Unpressed,
};


/// @brief A grid event which specifies the layout section location and event type.
struct grid_event_t : public grid_addr_t {
  /// @brief the type of the grid event
  GridEvent type;

  /// @brief Helper function for creating a grid event
  ///
  /// @param layout    the name of the layout associated with the grid event
  /// @param section   the section name associated with the grid event
  /// @param index     the 0-index of the led in the section associated with the grid event
  /// @param type      the type of the grid event
  ///
  static grid_event_t make(LayoutName layout,
                           GridSectionName section,
                           unsigned int index,
                           GridEvent type)
  {
    return {{
             .layout  = layout,
             .section = section,
             .index   = index,
            },
            .type     = type
    };
  };

  /// @brief Overloading the equality operator for grid event comparisons.
  bool operator==(const grid_event_t &m) const {
    return
      type    == m.type    &&
      layout  == m.layout  &&
      section == m.section &&
      index   == m.index;
  };
};

#endif

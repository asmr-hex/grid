#ifndef IO_GRID_H
#define IO_GRID_H

#include "anemone/io/grid/device/grid.hpp"
#include "anemone/io/grid/layout/context.hpp"


// what does this class do?
// has different layouts (which controllers can subscribe to)
// has a current layout (using the State pattern)...TODO how to implement?
// handles grid_device_events and delegates translation and broadcast to current layout
//   - handle must properly map grid_coordinates to grid_addr
// has an in-mem mapping of grid-coordinates -> grid section

class Grid : public Observer<grid_device_event_t> {
public:
  Grid(layout_initializer_list layouts, std::unique_ptr<GridDevice> device)
    : layout(layouts), device(device)
  {
    subscribe(*device);
  };
  // on handle grid_device_event, delegate event to layout (which will use the current layout and delegate to appropriate layout section (which are observables which will then broadcast (to controllers)))
  // on calls to update grid, translate to grid_coordinates using layout (which will use current layout and delegate to appropriate section to do translation), should return coordinates which are used by this class to send to GridDevice
  // on calls which use animator (like add animation, the addresses are translated and THEN stored in the animator) (animator has a shared_ptr of the GridDevice)
  //
  //
  // so layout must implement really two types of functionality:
  // (1) translate and broadcast grid_device_events from layout (to its observers (or more correctly, its current_layout's sections' observers))
  // (2) translate grid_addr (high abstraction) to grid_coordinates (low abstraction) and return
  LayoutContext layout;
private:
  std::unique_ptr<GridDevice> device;
};

#endif

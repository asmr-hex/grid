#ifndef IO_GRID_H
#define IO_GRID_H

#include <string>
#include <memory>

#include "anemone/config/config.hpp"
#include "anemone/io/observer.hpp"
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
  LayoutContext layout;
  
  Grid(std::shared_ptr<Config>, std::shared_ptr<GridDevice>, layout_initializer_list);

  // we need a connect method which subscribes the Grid to the GridDevice because
  // we cannot subscribe within the constructor. this is because the subscribe method
  // uses shared_from_this which assumes that there is a smart pointer managing this
  // object already, though by the time the constructor is called (even if it is called
  // wrapped within std::make_shared), the smart pointer has yet to be constructed itself.
  void connect();
  
  
  // on calls to update grid, translate to grid_coordinates using layout (which will use current layout and delegate to appropriate section to do translation), should return coordinates which are used by this class to send to GridDevice
  // TODO implement update grid (grid_event_t)
  
  // on calls which use animator (like add animation, the addresses are translated and THEN stored in the animator) (animator has a shared_ptr of the GridDevice)
  // TODO implement animator...

protected:
  // on handle grid_device_event, delegate event to layout (which will use the current layout and delegate to appropriate layout section (which are observables which will then broadcast (to controllers)))
  virtual void handle(const grid_device_event_t&) override;

private:
  std::string device_addr;
  std::shared_ptr<GridDevice> device;
};

#endif

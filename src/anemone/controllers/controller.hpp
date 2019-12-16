#ifndef CONTROLLERS_CONTROLLER_H
#define CONTROLLERS_CONTROLLER_H

#include <memory>

#include "anemone/io/event.hpp"
#include "anemone/io/layout/section.hpp"


// controler base class
// we want a controller to:
// (1) be an observer to grid_event_t
// (2) have access to the Grid (why?? do we??)
// (3) have access to the methods of the section it subscribes to (const member)
// (4) have acesss to the dispatcher (TBD)
// (5) direct (read-only) access to the state
class Controller : public Observer<grid_event_t> {
public:
  Controller(std::shared_ptr<GridSection> section)
    : section(section) {};

  void connect() {
    subscribe(*section);
  };
  
protected:
  std::shared_ptr<GridSection> section;

  virtual void handle(const grid_event_t&) = 0;
};

#endif

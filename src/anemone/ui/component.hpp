#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "anemone/rx/observer.hpp"


namespace ui {

  class Component : public rx::Observer {
  public:
    Component(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid) : state(state), grid(grid);
    virtual ~Component() = default;

    // all derived components must implement a connect function which is where they
    // should subscribe to the parts of the state tree they care about.
    virtual void connect() = 0;
    
    // all derived components must implement a render function which will be run
    // everytime the component must be re-rendered.
    virtual void render() = 0;

  protected:
    std::shared_ptr<Grid> grid;
    std::shared_ptr<State::Root> state;
    
    bool component_must_rerender = true;

    
  };

}

#endif

#ifndef UI_STEPS_H
#define UI_STEPS_H

#include "anemone/ui/component.hpp"


namespace ui {

  struct step_state {
    
  };
  
  class Steps : public Component {
  public:
    std::shared_ptr<Diff<step_state> > steps;
    
    Steps(std::shared_ptr<State::Root>, std::shared_ptr<Grid>);

    virtual void connect() override;
    
    virtual void render() override;
  };
  
}

#endif

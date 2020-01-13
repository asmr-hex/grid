#ifndef UI_STEPS_H
#define UI_STEPS_H

#include "anemone/ui/component.hpp"


namespace ui {

  class Steps : public Component {
  public:
    Steps(std::shared_ptr<State::Root>, std::shared_ptr<Grid>);

    virtual void connect() override;
    
    virtual void render() override;
  };
  
}

#endif

#include "anemone/ui/steps.hpp"


ui::Steps::Steps(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid)
  : Component(state, grid) {
  // do some stuff
}


void ui::Steps::connect() {
  // what should i connect to?
  subscribe<State::root>(state, [this] (State::root_t new_state) {
                                  
                                })
}

void ui::Steps::render() {
  
}

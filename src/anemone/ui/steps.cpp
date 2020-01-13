#include "anemone/ui/steps.hpp"


ui::Steps::Steps(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid)
  : Component(state, grid) {
  // do some stuff
}


void ui::Steps::connect() {
  subscribe<State::root_t, step_state>(state,
                                       steps,
                                       [] (State::root_t state) -> step_state {
                                         return {};
                                       });
}

void ui::Steps::render() {
  
}

#include "anemone/ui/steps.hpp"


ui::Steps::Steps(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid)
  : Component(state, grid) {
  // do some stuff
}


void ui::Steps::connect() {
  subscribe<State::root, step_state>(state,
                                     steps,
                                     [] (State::root state) -> step_state {
                                       return {};
                                     })
}

void ui::Steps::render() {
  
}

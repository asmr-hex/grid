#include "anemone/ui/ui.hpp"


UI::UI(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid) {
  steps = std::make_shared<ui::Steps>(state, grid);
}

void UI::connect_to_state() {
  steps->connect();
};

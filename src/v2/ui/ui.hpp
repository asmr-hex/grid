#ifndef UI_UI_H
#define UI_UI_H

#include <memory>

#include "anemone/state/root.hpp"
#include "anemone/io/grid/grid.hpp"

#include "anemone/ui/steps.hpp"


class UI {
public:
  std::shared_ptr<ui::Steps> steps;

  UI(std::shared_ptr<State::Root>, std::shared_ptr<Grid>);
  
  void connect_to_state();
};

#endif

/**
 * @file   ui/ui.hpp
 * @brief  Aggregate UI container
 * @author coco
 * @date   2020-02-19
 *************************************************/

#ifndef UI_UI_H
#define UI_UI_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/config.hpp"
#include "anemone/types.hpp"
#include "anemone/state.hpp"

#include "anemone/ui/shift.hpp"
#include "anemone/ui/step_sequence.hpp"


class UI {
public:
  UI(std::shared_ptr<Config>, std::shared_ptr<IO>, std::shared_ptr<State>);

  void connect();

private:
  std::shared_ptr<Config> config;
  std::shared_ptr<IO> io;
  std::shared_ptr<State> state;
    
  std::unique_ptr<ShiftUI> shift;
  std::unique_ptr<StepSequenceUI> step_sequence;
};

#endif

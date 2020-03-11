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
#include "anemone/ui/page.hpp"
#include "anemone/ui/parts.hpp"
#include "anemone/ui/banks.hpp"
#include "anemone/ui/ppqn.hpp"
#include "anemone/ui/play_pause.hpp"
#include "anemone/ui/stop.hpp"
#include "anemone/ui/show_last_step.hpp"


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
  std::unique_ptr<PageUI> pages;
  std::unique_ptr<PartsUI> parts;
  std::unique_ptr<BanksUI> banks;
  std::unique_ptr<PPQNUI> ppqn;
  std::unique_ptr<PlayPauseUI> play_pause;
  std::unique_ptr<StopUI> stop;
  std::unique_ptr<ShowLastStepUI> show_last_step;
};

#endif

/**
 * @file   controllers/sequencer/play_pause.hpp
 * @brief  Sequence Play/Pause Controller
 * @author coco
 * @date   2020-01-18
 *************************************************/


#ifndef ANEMONE_CONTROLLERS_SEQUENCER_PLAY_PAUSE_H
#define ANEMONE_CONTROLLERS_SEQUENCER_PLAY_PAUSE_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/state/root.hpp"
#include "anemone/action/dispatcher.hpp"

#include "anemone/controllers/base.hpp"


namespace ctrl {
  namespace sequencer {

    class PlayPause : public Controller<grid_event_t> {
    public:
      PlayPause(std::shared_ptr<IO> io,
            std::shared_ptr<GridLayouts> layouts,
            std::shared_ptr<State::Root> state,
            std::shared_ptr<Dispatcher> dispatcher,
            std::shared_ptr<GridSection> grid_section);

    private:
      virtual void handle(const grid_event_t&) override;
    };

  }
}

#endif

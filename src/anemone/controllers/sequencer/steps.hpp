/**
 * @file   controllers/sequencer/steps.hpp
 * @brief  Sequence Step Controller
 * @author coco
 * @date   2020-01-18
 *************************************************/


#ifndef CONTROLLERS_SEQUENCER_STEPS_H
#define CONTROLLERS_SEQUENCER_STEPS_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/state/root.hpp"
#include "anemone/action/dispatcher.hpp"

#include "anemone/controllers/base.hpp"


namespace ctrl {
  namespace sequencer {

    class Steps : public Controller<grid_event_t> {
    public:
      Steps(std::shared_ptr<IO> io,
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

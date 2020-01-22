#ifndef CONTROLLERS_CLOCK_SCHEDULER_H
#define CONTROLLERS_CLOCK_SCHEDULER_H

#include <memory>

#include "anemone/io.hpp"
#include "anemone/types.hpp"
#include "anemone/clock.hpp"
#include "anemone/actions.hpp"

#include "anemone/controllers/base.hpp"

#include "anemone/state/root.hpp"



namespace ctrl {
  namespace clock {

    class Scheduler : public Controller<types::tick_t> {
    public:
      Scheduler(std::shared_ptr<IO>,
                std::shared_ptr<GridLayouts>,
                std::shared_ptr<State::Root>,
                std::shared_ptr<Dispatcher>,
                std::shared_ptr<Clock>);

    private:
      virtual void handle(const types::tick_t&) override;
    };

  }
}


#endif

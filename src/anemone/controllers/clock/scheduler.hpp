#ifndef CONTROLLERS_CLOCK_SCHEDULER_H
#define CONTROLLERS_CLOCK_SCHEDULER_H

#include <memory>

#include "anemone/io/io.hpp"

#include "anemone/controllers/base.hpp"

#include "anemone/state/root.hpp"
#include "anemone/action/dispatcher.hpp"

#include "anemone/clock/clock.hpp"


namespace ctrl {
  namespace clock {

    class Scheduler : public Controller<tick_t> {
    public:
      Scheduler(std::shared_ptr<IO>,
                std::shared_ptr<State::Root>,
                std::shared_ptr<Dispatcher>,
                std::shared_ptr<Clock>);

    private:
      virtual void handle(const tick_t&) override;
    };

  }
}


#endif

#ifndef ACTIONS_DISPATCHER_H
#define ACTIONS_DISPATCHER_H

#include <vector>
#include <memory>

#include "anemone/action/types.hpp"
#include "anemone/util/concurrent_queue.hpp"


class Dispatcher {
public:
  Dispatcher(std::shared_ptr< Queue<action_t> >);
  void dispatch(const action_t& action);
  void dispatch(action_t&& action);
  void dispatch(std::vector<action_t> actions);

  // syncrhonous dispatch, blocks until action has been processed.
  void dispatch_wait  (action_t action);

private:
  std::shared_ptr< Queue<action_t> > queue;
};

#endif

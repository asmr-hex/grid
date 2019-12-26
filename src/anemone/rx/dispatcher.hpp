#ifndef RX_DISPATCHER_H
#define RX_DISPATCHER_H

#include <vector>
#include <memory>

#include "anemone/util/concurrent_queue.hpp"


namespace rx {

  template<typename Action>
  class Dispatcher {
  public:
    Dispatcher(std::shared_ptr< Queue<Action> >);
    void dispatch(const Action& action);
    void dispatch(Action&& action);
    void dispatch(std::vector<Action> actions);

    // syncrhonous dispatch, blocks until action has been processed.
    void dispatch_wait(Action action);

  private:
    std::shared_ptr< Queue<Action> > queue;
  };


  template<typename Action>
  Dispatcher<Action>::Dispatcher(std::shared_ptr< Queue<Action> > q)
    : queue(q) {};


  template<typename Action>
  void Dispatcher<Action>::dispatch(const Action& action) {
    queue->push(action);
  }

  template<typename Action>
  void Dispatcher<Action>::dispatch(Action&& action) {
    queue->push(std::move(action));
  }

  template<typename Action>
  void Dispatcher<Action>::dispatch(std::vector<Action> actions) {
    for (auto action : actions) {
      queue->push(std::move(action));
    } 
  }

  template<typename Action>
  void Dispatcher<Action>::dispatch_wait(Action action) {
    // TODO....
  }

}


#endif

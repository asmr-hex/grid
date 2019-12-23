#ifndef STATE_TREE_NODE_H
#define STATE_TREE_NODE_H

#include "anemone/state/state.hpp"
#include "anemone/action/types.hpp"


namespace State {

  template<typename T>
  class Observer;  // forward declaration
    
  namespace Tree {

    template<typename T>
    class Node {
    public:
      State::state_ptr<T, action_t> state;
      
      virtual ~Node() = default;
      
      void register_observer(const std::shared_ptr<State::Observer>&,
                             std::function<void(T)>);
    };
    
  }
}


#include "anemone/state/observer.hpp"

namespace State {
  namespace Tree {

    template<typename T>
    void Node<T>::register_observer(const std::shared_ptr<State::Observer>& observer,
                                    std::function<void(T)> handler) {
      state->register_observer(observer, handler);
    };

  }
}


#endif

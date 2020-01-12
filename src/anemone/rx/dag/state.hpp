#ifndef RX_DAG_STATE_H
#define RX_DAG_STATE_H

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/dag/observable.hpp"

#include <spdlog/spdlog.h>


namespace rx {
  namespace dag {

    template<typename T, typename Action>
    class State : public Node<T, Action>, public Observable<T> {
    public:
      State(std::unique_ptr<Node<T, Action> > node) : node(std::move(node)) {};
    
      virtual T get() override;
      virtual void reduce(Action) override;
    
    protected:
      std::unique_ptr<Node<T, Action> > node;
      bool dehydrated = true;
    };

  
    template<typename T, typename Action>
    T State<T, Action>::get() {
      return node->get();
    };

    template<typename T, typename Action>
    void State<T, Action>::reduce(Action action) {
      T old_value = node->get();

      node->reduce(action);

      T new_value = node->get();

      if ( !(new_value == old_value) || dehydrated ) {
        this->broadcast(new_value);
      }

      dehydrated = false;
    };

    
  }
}

#endif

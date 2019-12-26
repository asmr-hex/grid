#ifndef RX_DAG_LEAF_H
#define RX_DAG_LEAF_H

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/reducer.hpp"


namespace rx {
  namespace dag {

    template<typename T, typename Action>
    class Leaf : public Node<T, Action> {
    public:
      Leaf(reducer_fn_t<T, Action> fn, T t)
        :  state(t), reducer(fn) {};
  
      virtual T get() override;
      virtual void reduce(Action) override;
  
    private:
      T state;
      reducer_fn_t<T, Action> reducer;
    };


    template<typename T, typename Action>
    T Leaf<T, Action>::get() {
      return state;
    };

    template<typename T, typename Action>
    void Leaf<T, Action>::reduce(Action action) {
      state = reducer(state, action);
    };

    
  }
}

#endif

#ifndef STATE_LEAF_H
#define STATE_LEAF_H

#include "anemone/state/node.hpp"
#include "anemone/state/reducer.hpp"


namespace State {

  template<typename T, typename A>
  class Leaf : public Node<T, A> {
  public:
    Leaf(reducer_fn_t<T, A> fn, T t)
      :  state(t), reducer(fn) {};
  
    virtual T value() override;
    virtual void reduce(A) override;
  
  private:
    T state;
    reducer_fn_t<T, A> reducer;
  };


  template<typename T, typename A>
  T Leaf<T, A>::value() {
    return state;
  };

  template<typename T, typename A>
  void Leaf<T, A>::reduce(A a) {
    state = reducer(state, a);
  };

  
}

#endif

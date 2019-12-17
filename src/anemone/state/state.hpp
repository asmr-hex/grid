#ifndef STATE_H
#define STATE_H

#include <memory>
#include <utility>

#include "anemone/state/node.hpp"
#include "anemone/state/reducer.hpp"
#include "anemone/state/leaf.hpp"
#include "anemone/state/composition.hpp"


template<typename T>
struct type_identity {
  using type = T;
};


namespace State {

  template<typename T>
  struct State {
    template<typename A>
    state_t<T, A> static with_reducer(typename type_identity<reducer_fn_t<T, A> >::type fn, T t) {
      return state_t<T, A>(std::make_unique<Leaf<T, A> >(fn, t));
    };

    template<typename A>
    struct with_actions {
      template<typename...S>
      state_t<T, A> static compose(typename type_identity<compose_fn_t<T, S...> >::type fn, std::shared_ptr<State<S> >...s) {
        return state_t<T, A>(std::make_unique<Composition<T, A, S...> >(fn, s...));
      };
    };

  };
  
  template<typename T, typename A>
  class state_t : public Node<T, A> {
  public:
    state_t(std::unique_ptr<Node<T, A> > node) : node(std::move(node)) {};
    
    virtual T value() override;
    virtual void reduce(A) override;
    
  protected:
    std::unique_ptr<Node<T, A> > node;
  };

  
  template<typename T, typename A>
  T state_t<T, A>::value() {
    return node->value();
  };

  template<typename T, typename A>
  void state_t<T, A>::reduce(A a) {
    node->reduce(a);
  };

}

#endif

#ifndef STATE_H
#define STATE_H

#include <memory>
#include <utility>

#include "anemone/state/node.hpp"
#include "anemone/state/reducer.hpp"
#include "anemone/state/leaf.hpp"
#include "anemone/state/composition.hpp"
#include "anemone/state/observable.hpp"


template<typename T>
struct type_identity {
  using type = T;
};

namespace State {

  template<typename T>
  struct State {
    template<typename A>
    std::shared_ptr<state_t<T, A> >
    static with_reducer(T initial_state, typename type_identity<reducer_fn_t<T, A> >::type fn)
    {
      return std::make_shared<state_t<T, A> >
        (std::make_unique<Leaf<T, A> >(fn, initial_state));
    };

    template<typename A>
    struct with_actions {
      template<typename...S>
      static std::shared_ptr<state_t<T, A> >
      compose(typename type_identity<compose_fn_t<T, S...> >::type fn, std::shared_ptr<state_t<S, A> >...s) {
        return std::make_shared<state_t<T, A> >
          (std::make_unique<Composition<T, A, S...> >(fn, s...));
      };
    };
  };

  
  template<typename T, typename A>
  class state_t : public Node<T, A>, public Observable<T> {
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
    T old_value = node->value();
    
    node->reduce(a);
    
    if ( !(node->value() == old_value) )
      this->broadcast(node->value());
  };

}

#endif

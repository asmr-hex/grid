#ifndef STATE_COMPOSITION_H
#define STATE_COMPOSITION_H

#include <memory>
#include <functional>

#include "anemone/state/node.hpp"


namespace State {
  
  template<typename T, typename...S>
  using compose_fn_t = std::function<T(S...)>;

  template<typename T, typename A>
  class state_t;  // forward declaration

  
  template<typename T, typename A, typename...S>
  class Composition : public Node<T, A> {
  public:
    Composition(compose_fn_t<T, S...> composer, std::shared_ptr<state_t<S, A> >...s);

    virtual T value() override;
    virtual void reduce(A) override;
    
  private:
    T state;
    std::function<T()> compose;
    // std::vector< std::variant<Node<S> *...> > nodes;

    void curry(compose_fn_t<T, S...> fn, std::shared_ptr<state_t<S, A> >...s);
  };

}


#include "anemone/state/state.hpp"


namespace State {

  
  template<typename T, typename A, typename...S>
  Composition<T, A, S...>::Composition(compose_fn_t<T, S...> fn, std::shared_ptr<state_t<S, A> >...s) {
    curry(fn, s...);
  }

  template<typename T, typename A, typename...S>
  T Composition<T, A, S...>::value() {
    return state;
  };

  template<typename T, typename A, typename...S>
  void Composition<T, A, S...>::reduce(A a) {
    // reduce over all sub states

    state = compose();
  };

  template<typename T, typename A, typename...S>
  void Composition<T, A, S...>::curry(compose_fn_t<T, S...> fn, std::shared_ptr<state_t<S, A> >...s) {
    compose = [fn, s...] () -> T { return fn(s->value()...); };
  }
  
};


#endif

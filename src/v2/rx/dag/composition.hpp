#ifndef RX_DAG_COMPOSITION_H
#define RX_DAG_COMPOSITION_H

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/types.hpp"


namespace rx {
  namespace dag{


    template<typename T, typename A>
    class State;  // forward declaration

    
    template<typename T, typename...S>
    using compose_fn_t = std::function<T(S...)>;

    
    template<typename T, typename A, typename...S>
    class Composition : public Node<T, A> {
    public:
      Composition(compose_fn_t<T, S...> composer, types::state_ptr<S, A>...s);

      virtual T get() override;
      virtual void reduce(A) override;
    
    private:
      T state;
      std::function<T()> compose;
      std::vector<std::shared_ptr<Reducible<A> > > nodes;

      void curry(compose_fn_t<T, S...> fn, types::state_ptr<S, A>...s);
    };

    
  }
}


#include "anemone/rx/dag/state.hpp"


namespace rx {
  namespace dag {

    template<typename T, typename A, typename...S>
    Composition<T, A, S...>::Composition(compose_fn_t<T, S...> fn, types::state_ptr<S, A>...s)
      : nodes({s...}) {
      curry(fn, s...);
      state = compose();
    };

    template<typename T, typename A, typename...S>
    T Composition<T, A, S...>::get() {
      return state;
    };

    template<typename T, typename A, typename...S>
    void Composition<T, A, S...>::reduce(A a) {
      for (auto node : nodes) {
        node->reduce(a);
      }

      state = compose();
    };

    template<typename T, typename A, typename...S>
    void Composition<T, A, S...>::curry(compose_fn_t<T, S...> fn, types::state_ptr<S, A>...s) {
      compose = [fn, s...] () -> T { return fn(s->get()...); };
    };
    
  }
}

#endif

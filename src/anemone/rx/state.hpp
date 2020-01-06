#ifndef RX_STATE_H
#define RX_STATE_H

#include <vector>

#include "anemone/util/type_identity.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/reducer.hpp"
#include "anemone/rx/dag/composition.hpp"
#include "anemone/rx/dag/vector_composition.hpp"
#include "anemone/rx/dag/leaf.hpp"
#include "anemone/rx/dag/state.hpp"


namespace rx {

  template<typename T>
  struct State {
    
    template<typename A>
    static types::state_ptr<T, A>
    with_reducer(T initial_state, typename type_identity<reducer_fn_t<T, A> >::type fn)
    {
      return std::make_shared<dag::State<T, A> >
        (std::make_unique<dag::Leaf<T, A> >(fn, initial_state));
    };

    template<typename A>
    struct with_actions {

      // compose non-vector states
      template<typename...S>
      static types::state_ptr<T, A>
      compose(typename type_identity<dag::compose_fn_t<T, S...> >::type fn,
              types::state_ptr<S, A>...s)
      {
        return std::make_shared<dag::State<T, A> >
          (std::make_unique<dag::Composition<T, A, S...> >(fn, s...));
      };

      // compose vector states
      static types::state_ptr<T, A>
      compose_vector(std::vector<types::state_ptr<T, A> > v,
                     dag::filter_fn_t<A> fn = ([] (A a) -> std::vector<int> { return {}; }) )
      {
        return std::make_shared<dag::State<std::vector<T> *, A> >
          (std::make_unique<dag::VectorComposition<T, A> >(v, fn));
      };
      
    };
  };

}

#endif

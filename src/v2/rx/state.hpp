#ifndef RX_STATE_H
#define RX_STATE_H

#include <memory>
#include <vector>
#include <functional>

#include "anemone/util/type_identity.hpp"
#include "anemone/rx/types.hpp"
#include "anemone/rx/reducer.hpp"
#include "anemone/rx/dag/composition.hpp"
#include "anemone/rx/dag/map_composition.hpp"
#include "anemone/rx/dag/vector_composition.hpp"
#include "anemone/rx/dag/leaf.hpp"
#include "anemone/rx/dag/state.hpp"
#include "anemone/rx/dag/high_frequency_state.hpp"


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
      static types::state_ptr<std::vector<T> *, A>
      compose_vector(std::vector<types::state_ptr<T, A> > v,
                     dag::filter_fn_t<A> fn = ([] (A a) -> std::vector<int> { return {}; }) )
      {
        return std::make_shared<dag::State<std::vector<T> *, A> >
          (std::make_unique<dag::VectorComposition<T, A> >(v, fn));
      };

      // compose map states
      template<typename K>
      static types::state_ptr<std::map<K, T> *, A>
      compose_map(std::map<k, types::state_ptr<T, A> > m,
                  map_filter_fn_t<A, K> fn = ([] (A a) -> std::vector<K> { return {}; }) )
      {
        return std::make_shared<dag::State<std::map<K, T> *, A> >
          (std::make_unique<dag::MapComposition<T, A, K> >(m, fn));
      };
    };
  };


  struct HighFrequency {
    template<typename T>
    struct State {
    
      template<typename A>
      static types::high_frequency::state_ptr<T, A>
      with_reducer(T initial_state,
                   typename type_identity<high_freq_reducer_fn_t<T, A> >::type fn,
                   typename type_identity<std::function<bool(A)> >::type pred)
      {
        return std::make_shared<dag::high_frequency::State<T, A> >(fn, pred, initial_state);
      };
    };    
  };

}

#endif

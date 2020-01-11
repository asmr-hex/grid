#ifndef RX_DAG_HIGH_FREQUENCY_LEAF_H
#define RX_DAG_HIGH_FREQUENCY_LEAF_H

#include <memory>
#include <functional>

#include "anemone/rx/reducer.hpp"


// TODO: make this derive from Observable, In order to be efficient, include
// a function which broadcasts the pointer result! this will ensure that we aren't
// equality comparing large datastructures upon every high frequency action-- i.e. it
// gives more fine grained control over when we choose to broadcast!
// THIS should actually be a broadcast predicate function!
//
// in rx::HighFrequency::State call, we should NOT wrap this HighFrequency Leaf in
// a rx::dag::State because it doesn't derive from Node...in fact, we don't even need
// a get method any more!

namespace rx {
  namespace dag {
    namespace high_frequency {

      template<typename T, typename Action>
      class State : public Observable<std::shared_ptr<T> > {
      public:
        State(high_freq_reducer_fn_t<T, Action> fn, std::function<bool(Action)> pred, T t)
          : reducer(fn), broadcast_predicate(pred) {
          state = std::make_shared<T>(T(t));
        };

        std::shared_ptr<T> get();
        void reduce(Action);
  
      private:
        std::shared_ptr<T> state;
        high_freq_reducer_fn_t<T, Action> reducer;
        std::function<bool(Action)> broadcast_predicate;
      };


      template<typename T, typename Action>
      std::shared_ptr<T> State<T, Action>::get() {
        return state;
      };

      template<typename T, typename Action>
      void State<T, Action>::reduce(Action action) {
        reducer(state, action);

        if (broadcast_predicate(action))
          this->broadcast(state);
      };
    }
  }
}

#endif

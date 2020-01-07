#ifndef RX_DAG_VECTOR_COMPOSITION_H
#define RX_DAG_VECTOR_COMPOSITION_H

#include <vector>

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/types.hpp"

#include <spdlog/spdlog.h>


namespace rx {
  namespace dag{


    template<typename T, typename A>
    class State;  // forward declaration

    
    template<typename A>
    using filter_fn_t = std::function<std::vector<int>(A)>;

    
    template<typename T, typename A>
    class VectorComposition : public Node<std::vector<T> *, A> {
    public:
      VectorComposition(std::vector<types::state_ptr<T, A> >);
      VectorComposition(std::vector<types::state_ptr<T, A> >, filter_fn_t<A>);

      virtual std::vector<T> *get() override;
      virtual void reduce(A) override;
    
    private:
      std::vector<T> *state;
      filter_fn_t<A> filter;
      std::vector<types::state_ptr<T, A> > nodes;

      void compose();
    };

    
  }
}


#include "anemone/rx/dag/state.hpp"


namespace rx {
  namespace dag {

    template<typename T, typename A>
    VectorComposition<T, A>::VectorComposition(std::vector<types::state_ptr<T, A> > v,
                                               filter_fn_t<A> fn)
      : filter(fn), nodes(v) {
      state = new std::vector<T>();
      state->reserve(nodes.size());
      
      compose();
    };

    template<typename T, typename A>
    void VectorComposition<T, A>::compose() {
      for (int i = 0; i < nodes.size(); i++) {
        state->insert(state->begin() + i, nodes[i]->get());
      }      
    }

    template<typename T, typename A>
    std::vector<T> *VectorComposition<T, A>::get() {
      return state;
    };

    template<typename T, typename A>
    void VectorComposition<T, A>::reduce(A a) {
      for (auto node : nodes) {
        node->reduce(a);
      }

      compose();
    };
  }
}

#endif

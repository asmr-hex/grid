/**
 * @file   rx/dag/map_composition.hpp
 * @brief  Reactive DAG Map Composition Node
 * @author coco
 * @date   2020-02-01
 *************************************************/


#ifndef RX_DAG_MAP_COMPOSITION_H
#define RX_DAG_MAP_COMPOSITION_H


#include <map>
#include <vector>
#include <functional>

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/types.hpp"


namespace rx {
  namespace dag {


    template<typename T, typename A>
    class State;  // forward declaration


    template<typename A, typename K>
    using map_filter_fn_t = std::function<std::vector<K>(A)>;


    template<typename T, typename A, typename K>
    class MapComposition : public Node<std::map<K, T> *, A> {
    public:
      MapComposition(std::map<k, types::state_ptr<T, A> >);
      MapComposition(std::map<k, types::state_ptr<T, A> >, map_filter_fn_t<A, K>);

      virtual std::map<K, T> *get() override;
      virtual void reduce(A) override;
    private:
      std::map<K, T> *state;
      map_filter_fn_t<A, K> filter;
      std::map<K, types::state_ptr<T, A> > nodes;

      void compose();
    };
  }
}


#include "anemone/rx/dag/state.hpp"


namespace rx {
  namespace dag {

    template<typename T, typename A, typename K>
    MapComposition<T, A, K>::MapComposition(std::map<k, types::state_ptr<T, A> > m,
                                            map_filter_fn_t<A, K> fn)
      : filter(fn), nodes(m) {
      state = new std::map<K, T>();
      
      compose();
    };

    template<typename T, typename A, typename K>
    void MapComposition<T, A, K>::compose() {
      for (auto itr : nodes) {
        state->insert(itr.first, itr.second->get());
      }
    };

    template<typename T, typename A, typename K>
    std::map<K, T> * MapComposition<T, A, K>::get() {
      return state;
    };

    template<typename T, typename A, typename K>
    void MapComposition<T, A, K>::reduce(A a) {
      // only reduce nodes whoes keys re returned by filter
      for (auto key : filter(a)) {
        nodes->at(key)->reduce(a);
      }

      compose();
    };
  }
}

#endif

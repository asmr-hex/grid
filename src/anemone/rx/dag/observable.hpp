#ifndef RX_DAG_OBSERVABLE_H
#define RX_DAG_OBSERVABLE_H

#include <vector>
#include <memory>
#include <functional>

#include "anemone/rx/filter.hpp"


namespace rx {
  
  template<typename T>
  class Observable;  // forward declaration
  
  namespace dag {

    
    template<typename T>
    class Observable  {
    public:
      void broadcast(const T&);

      template<typename S>
      void register_observer(std::function<void(S)>, std::shared_ptr<Filter<T, S> >);

    protected:
      std::vector< std::function<void(T)> > observers;

      friend class rx::Observable<T>;
    };


    template<typename T>
    void Observable<T>::broadcast(const T& t) {
      for (auto observer : observers) {
        std::function<void(T)> handler = observer;
        handler(t);
      }
    };

    template<typename T>
    template<typename S>
    void Observable<T>::register_observer(std::function<void(S)> handler,
                                          std::shared_ptr<Filter<T, S> > filter) {
      observers.push_back([filter, handler] (T t) -> void {
                            // transform data with filter
                            S s = filter->filter(t);

                            // only execute handler if predicate is truthy
                            if ( filter->predicate(s) )
                              handler(s);
                          });
    }
  }
}

#endif

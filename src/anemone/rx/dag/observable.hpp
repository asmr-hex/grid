#ifndef RX_DAG_OBSERVABLE_H
#define RX_DAG_OBSERVABLE_H

#include <vector>
#include <memory>
#include <functional>

#include "anemone/rx/predicate.hpp"


namespace rx {
  
  template<typename T>
  class Observable;  // forward declaration
  
  namespace dag {

    
    template<typename T>
    class Observable  {
    public:
      void broadcast(const T&);
      void register_observer(std::function<void(T)>, std::shared_ptr<Predicate<T> >);

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
    void Observable<T>::register_observer(std::function<void(T)> handler,
                                          std::shared_ptr<Predicate<T> > predicate) {
      // wrap handler and predicate in a lambda s.t. handler is only executed if predicate returns true
      observers.push_back([predicate, handler] (T t) -> void {
                            if ( (*predicate)(t) )
                              handler(t);
                          });
    }
  }
}

#endif

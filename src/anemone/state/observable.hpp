#ifndef STATE_OBSERVABLE_H
#define STATE_OBSERVABLE_H

#include <map>
#include <memory>
#include <functional>

namespace State {

  class Observer;  // forward declaration
  
  template<typename T>
  class Observable  {
  public:
    void broadcast(const T&);
    void register_observer(const std::shared_ptr<Observer>&,
                           std::function<void(T)>);

  protected:
    std::map< std::shared_ptr<Observer>,
             std::function<void(T)> > observers;
  };

}

#include "anemone/state/observer.hpp"

namespace State {

  template<typename T>
  void Observable<T>::broadcast(const T& t) {
    for (auto observer : observers) {
      std::function<void(T)> handler = observer.second;
      handler(t);
    }
  };

  template<typename T>
  void Observable<T>::register_observer(const std::shared_ptr<Observer>& observer,
                                        std::function<void(T)> handler) {
    observers[observer] = handler;
  }
}

#endif

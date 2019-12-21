#ifndef IO_OBSERVER_H
#define IO_OBSERVER_H

#include <memory>
#include <functional>


namespace State {

  template<typename T>
  class Observable;  // forward declaration

  class Observer : public std::enable_shared_from_this<Observer> {
  public:
    template<typename T>
    void subscribe(std::shared_ptr<Observable<T> >, std::function<void(T)>);
  };
}

#include "anemone/state/observable.hpp"

namespace State {
  template<typename T>
  void Observer::subscribe(std::shared_ptr<Observable<T> > observable, std::function<void(T)> fn) {
    observable->register_observer(this->shared_from_this(), fn);
  };
}

#endif

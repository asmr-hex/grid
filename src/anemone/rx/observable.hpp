#ifndef RX_OBSERVABLE_H
#define RX_OBSERVABLE_H

#include <functional>

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/dag/observable.hpp"


namespace rx {

  // rx::Observable wraps an rx::dag::Observable
  template<typename T>
  class Observable {
  public:
    virtual ~Observable() = default;

  protected:
    virtual std::shared_ptr< dag::Observable<T> > get() = 0;

  private:
    void register_observer(std::function<void(T)>);

    friend class Observer;
  };


  template<typename T>
  void Observable<T>::register_observer(std::function<void(T)> handler) {
    get()->register_observer(handler);
  };
  
}

#endif

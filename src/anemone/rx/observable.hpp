#ifndef RX_OBSERVABLE_H
#define RX_OBSERVABLE_H

#include <functional>

#include "anemone/rx/dag/node.hpp"
#include "anemone/rx/dag/observable.hpp"

#include "anemone/rx/filter.hpp"


namespace rx {

  // rx::Observable wraps an rx::dag::Observable
  template<typename T>
  class Observable {
  public:
    virtual ~Observable() = default;

  protected:
    virtual std::shared_ptr< dag::Observable<T> > get() = 0;

  private:
    template<typename S>
    void register_observer(std::function<void(S)>, std::shared_ptr<Filter<T, S> >);

    friend class Observer;
  };


  template<typename T>
  template<typename S>
  void Observable<T>::register_observer(std::function<void(S)> handler,
                                        std::shared_ptr<Filter<T, S> > filter) {
    get()->template register_observer<T, S>(handler, filter);
  };
  
}

#endif

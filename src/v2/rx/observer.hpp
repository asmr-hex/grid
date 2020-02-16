#ifndef RX_OBSERVER_H
#define RX_OBSERVER_H

#include <memory>
#include <functional>

#include "anemone/rx/filter.hpp"


namespace rx {


  template<typename T>
  class Observable;  // forward declaration

  namespace dag {
    template<typename T>
    class Observable;  // forward declaration
  }


  // an rx::Observer can subscribe to an rx::Observable or an rx::dag::Observable
  // directly. recall that rx::Observable is a wrapper over an rx::dag::Observable.
  class Observer {
  public:
    template<typename T>
    void subscribe(std::shared_ptr<Observable<T> >, std::function<void(T)>);

    template<typename T>
    void subscribe(std::shared_ptr<Observable<T> >, std::function<void(T)>, std::shared_ptr<Filter<T, T> >);
    
    template<typename T, typename S>
    void subscribe(std::shared_ptr<Observable<T> >, std::function<void(S)>, std::shared_ptr<Filter<T, S> >);

    
    template<typename T>
    void subscribe(std::shared_ptr<dag::Observable<T> >, std::function<void(T)>);

    template<typename T>
    void subscribe(std::shared_ptr<dag::Observable<T> >, std::function<void(T)>, std::shared_ptr<Filter<T, T> >);
    
    template<typename T, typename S>
    void subscribe(std::shared_ptr<dag::Observable<T> >, std::function<void(S)>, std::shared_ptr<Filter<T, S> >);
  };
  
}


#include "anemone/rx/observable.hpp"
#include "anemone/rx/dag/observable.hpp"


namespace rx {

  template<typename T>
  void Observer::subscribe(std::shared_ptr<Observable<T> > observable, std::function<void(T)> fn) {
    std::shared_ptr<Filter<T, T> > filter = std::make_shared<DefaultFilter<T> >();
    observable->template register_observer<T>(fn, filter);
  };

  template<typename T>
  void Observer::subscribe(std::shared_ptr<Observable<T> > observable,
                           std::function<void(T)> fn,
                           std::shared_ptr<Filter<T, T> > filter) {
    observable->template register_observer<T>(fn, filter);
  };

  template<typename T, typename S>
  void Observer::subscribe(std::shared_ptr<Observable<T> > observable,
                           std::function<void(S)> fn,
                           std::shared_ptr<Filter<T, S> > filter) {
    observable->template register_observer<S>(fn, filter);
  };

  
  template<typename T>
  void Observer::subscribe(std::shared_ptr<dag::Observable<T> > observable, std::function<void(T)> fn) {
    std::shared_ptr<Filter<T, T> > filter = std::make_shared<DefaultFilter<T> >();
    observable->template register_observer<T>(fn, filter);
  };

  template<typename T>
  void Observer::subscribe(std::shared_ptr<dag::Observable<T> > observable,
                           std::function<void(T)> fn,
                           std::shared_ptr<Filter<T, T> > filter) {
    observable->template register_observer<T>(fn, filter);
  };

  template<typename T, typename S>
  void Observer::subscribe(std::shared_ptr<dag::Observable<T> > observable,
                           std::function<void(S)> fn,
                           std::shared_ptr<Filter<T, S> > filter) {
    observable->template register_observer<S>(fn, filter);
  };
}

#endif

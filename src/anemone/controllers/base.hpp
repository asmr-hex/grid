#ifndef CONTROLLERS_BASE_H
#define CONTROLLERS_BASE_H

#include <memory>

#include "anemone/io/observer.hpp"
#include "anemone/io/observable.hpp"
#include "anemone/rx/observer.hpp"

#include "anemone/state/root.hpp"
#include "anemone/action/dispatcher.hpp"


template<typename T>
class Controller : public Observer<T>, public rx::Observer {
public:
  Controller(std::shared_ptr<State::Root>,
             std::shared_ptr<Dispatcher>,
             std::shared_ptr<Observable<T> >);

  void listen();
protected:
  State::root_t state;
  std::shared_ptr<Dispatcher> dispatcher;
  std::shared_ptr<Observable<T> > observable;

  virtual void handle(const T&) = 0;
};


template<typename T>
Controller<T>::Controller(std::shared_ptr<State::Root> state_p,
                          std::shared_ptr<Dispatcher> dispatcher,
                          std::shared_ptr<Observable<T> > observable)
  : dispatcher(dispatcher), observable(observable) {
  subscribe<State::root_t>(state_p, [this] (State::root_t s) {
                                      state = s;
                                    });
};


template<typename T>
void Controller<T>::listen() {
  ::Observer<T>::subscribe(*observable);
};

#endif

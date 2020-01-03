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
  std::shared_ptr<State::Root> state_wrapper;
  std::shared_ptr<Dispatcher> dispatcher;
  std::shared_ptr<Observable<T> > observable;

  virtual void handle(const T&) = 0;
};


template<typename T>
Controller<T>::Controller(std::shared_ptr<State::Root> state_wrapper,
                          std::shared_ptr<Dispatcher> dispatcher,
                          std::shared_ptr<Observable<T> > observable)
  : state_wrapper(state_wrapper), dispatcher(dispatcher), observable(observable) {};


template<typename T>
void Controller<T>::listen() {
  // listen to state changes
  subscribe<State::root_t>(state_wrapper, [this] (State::root_t s) {
                                            state = s;
                                          });

  // listen to io/clock observable
  ::Observer<T>::subscribe(*observable);
};

#endif

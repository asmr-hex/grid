#ifndef CONTROLLERS_BASE_H
#define CONTROLLERS_BASE_H

#include <memory>
#include <vector>

#include "anemone/io/io.hpp"
#include "anemone/io/observer.hpp"
#include "anemone/io/observable.hpp"
#include "anemone/rx/observer.hpp"
#include "anemone/io/grid/layout/layouts/layouts.hpp"

#include "anemone/state/root.hpp"
#include "anemone/action/types.hpp"
#include "anemone/action/dispatcher.hpp"
#include "anemone/action/creators/creators.hpp"

#include <spdlog/spdlog.h>


template<typename T>
class Controller : public Observer<T>, public rx::Observer {
public:
  action::Creators make_action;

  Controller(std::shared_ptr<IO>,
             std::shared_ptr<GridLayouts>,
             std::shared_ptr<State::Root>,
             std::shared_ptr<Dispatcher>,
             std::shared_ptr<Observable<T> >);

  void listen();
  void dispatch(action_t);
  void dispatch(high_freq_action_t);
  void dispatch(std::vector<action_t>);
protected:
  State::root_t state;
  std::shared_ptr<State::Root> state_wrapper;
  std::shared_ptr<Dispatcher> dispatcher;
  std::shared_ptr<Observable<T> > observable;

  virtual void handle(const T&) = 0;
};


template<typename T>
Controller<T>::Controller(std::shared_ptr<IO> io,
                          std::shared_ptr<GridLayouts> layouts,
                          std::shared_ptr<State::Root> state_wrapper,
                          std::shared_ptr<Dispatcher> dispatcher,
                          std::shared_ptr<Observable<T> > observable)
  : make_action(io, layouts), state_wrapper(state_wrapper), dispatcher(dispatcher), observable(observable) {};


template<typename T>
void Controller<T>::listen() {
  // listen to state changes
  subscribe<State::root_t>(state_wrapper, [this] (State::root_t s) {
                                            make_action.state = s;
                                            state = s;
                                          });

  // listen to io/clock observable
  ::Observer<T>::subscribe(*observable);
};

template<typename T>
void Controller<T>::dispatch(action_t action_) {
  dispatcher->dispatch(action_);
};

template<typename T>
void Controller<T>::dispatch(high_freq_action_t action_) {
  dispatcher->dispatch(action_);
};


template<typename T>
void Controller<T>::dispatch(std::vector<action_t> actions) {
  dispatcher->dispatch(actions);
};

#endif

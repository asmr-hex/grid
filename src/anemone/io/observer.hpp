/**
 * @file   io/observer.hpp
 * @brief  Observer template class for IO
 * @author coco
 * @date   2020-01-16
 *************************************************/


#ifndef IO_OBSERVER_H
#define IO_OBSERVER_H

#include <memory>


// forward declaration
template<typename Event>
class Observable;


/// @brief Template class for creating an I/O observer.
///
/// @tparam Event   Any type; emitted as an event by an IO object
///
///
/// @details
/// An `Observer<Event>` exposes two public methods for subscribing/unsibscribing
/// to a `Observable<Event>` and has a protected pure virtual method for handling
/// events received upon an `Observable<Event>` broadcast. Thus, the only way to
/// use this class is through inheritance. Also, since the subscribe/unsubscribe
/// methods are wrappers over calling the register/unregister methods of the
/// `Observable<Event>` of interest, the derived class must be wrapped in a `shared_ptr`
///
template<typename Event>
class Observer : public std::enable_shared_from_this< Observer<Event> > {
public:
  virtual ~Observer() = default;
  
  void subscribe(Observable<Event>&);
  void unsubscribe(Observable<Event>&);
protected:
  virtual void handle(const Event&) = 0;

  friend class Observable<Event>;
};


#include "observable.hpp"

// since there is a circular dependency between the Observable & Observer
// templates, and both need to access methods of the other, we are using
// this forward-declaration + postponed import approach:
//
// (1) forward-declare the dependency so this type definition is complete.
// (2) only import the dependency when we are implementing this type's methods
//
// the result is that the above type definition is complete so when the
// dependency includes this file, it will have access to the complete definition
// before it is included itself. the header guards ensure it isn't included again.


template<typename Event>
void Observer<Event>::subscribe(Observable<Event>& observable) {
  observable.register_observer(this->shared_from_this());
};


template<typename Event>
void Observer<Event>::unsubscribe(Observable<Event>& observable) {
  observable.unregister_observer(this->shared_from_this());
};

#endif

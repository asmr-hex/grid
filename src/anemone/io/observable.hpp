/**
 * @file   io/observable.hpp
 * @brief  Observable template class for IO
 * @author coco
 * @date   2020-01-15
 *************************************************/


#ifndef IO_OBSERVABLE_H
#define IO_OBSERVABLE_H

#include <set>
#include <memory>


// forward declaration
template<typename Event>
class Observer;


/// @brief Template class for creating an I/O observable.
///
/// @tparam Event   Any type; emitted as an event by an IO object
///
/// @details
/// An Observable<Event> exposes methods for registering/unregistering observers
/// as well as for broadcasting events to all registered observers. To use an
/// Observable<Event>, you can either create a derived class or just instantiate
/// an Observable<Event> directly since it contains no virtual methods.
///
/// Example
/// @code
///   // define a class which is derived from an int Observable
///   class ObservableInt : public Observable<int> {
///   public:
///     void count_to_five() {
///       for (int i = 0; i < 5; i++) {
///         // this will send the value of i to any observers that have
///         // registered to recieve updates from this ObservableInt.
///         broadcast(i);
///       }
///     };
///   }
/// @endcode
///
template<typename Event>
class Observable {
public:
  /// @brief broadcasts an event to all registered observers.
  ///
  /// @param event   the event to broadcast
  ///
  void broadcast(const Event& event);

  /// @brief registers an observer to receive events upon broadcast.
  ///
  /// @param observer   the observer to subscribe to broadcasts
  ///
  void register_observer(const std::shared_ptr< Observer<Event> >&);

  /// @brief unregisters an observer from receiving events upon broadcast.
  ///
  /// @param observer   the observer to unsibscribe from broadcasts
  ///
  void unregister_observer(const std::shared_ptr< Observer<Event> >&);

protected:
  std::set< std::shared_ptr< Observer<Event> > > observers;
};


#include "observer.hpp"

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
void Observable<Event>::broadcast(const Event& event) {
  for (auto observer : observers) {
    observer->handle(event);
  }
};


template<typename Event>
void Observable<Event>::register_observer(const std::shared_ptr< Observer<Event> >& observer) {
  observers.insert(observer);
};


template<typename Event>
void Observable<Event>::unregister_observer(const std::shared_ptr< Observer<Event> >& observer) {
  observers.erase(observer);
};


#endif

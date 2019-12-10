#ifndef IO_OBSERVABLE_H
#define IO_OBSERVABLE_H

#include <set>
#include <memory>


template<typename Event>
class Observer;


template<typename Event>
class Observable {
public:
  void notify(const Event&);
  void register_observer(std::shared_ptr< Observer<Event> >&);
  void unregister_observer(std::shared_ptr< Observer<Event> >&);

private:
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
void Observable<Event>::notify(const Event& event) {
  for (auto observer : observers) {
    observer->handle(event);
  }
};


template<typename Event>
void Observable<Event>::register_observer(std::shared_ptr< Observer<Event> >& observer) {
  observers.insert(observer);
};


template<typename Event>
void Observable<Event>::unregister_observer(std::shared_ptr< Observer<Event> >& observer) {
  observers.erase(observer);
};

#endif

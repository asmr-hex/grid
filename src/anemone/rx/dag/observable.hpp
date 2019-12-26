#ifndef RX_DAG_OBSERVABLE_H
#define RX_DAG_OBSERVABLE_H

#include <vector>
#include <memory>
#include <functional>


namespace rx {
  
  template<typename T>
  class Observable;  // forward declaration
  
  namespace dag {

    
    template<typename T>
    class Observable  {
    public:
      void broadcast(const T&);
      void register_observer(std::function<void(T)>);

    protected:
      std::vector< std::function<void(T)> > observers;

      friend class rx::Observable<T>;
    };


    template<typename T>
    void Observable<T>::broadcast(const T& t) {
      for (auto observer : observers) {
        std::function<void(T)> handler = observer;
        handler(t);
      }
    };

    template<typename T>
    void Observable<T>::register_observer(std::function<void(T)> handler) {
      observers.push_back(handler);
    }
  }
}

#endif

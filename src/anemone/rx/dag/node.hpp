#ifndef RX_DAG_NODE_H
#define RX_DAG_NODE_H

namespace rx {

  namespace dag {

    // a Reducible must implement a reduce method which takes a specific
    // action type.
    template<typename Action>
    class Reducible {
    public:
      virtual ~Reducible() = default;
    
      virtual void reduce(Action) = 0;
    };

    // a Readable implements a get method to read the current state.
    template<typename T>
    class Readable {
    public:
      virtual ~Readable() = default;

      virtual T get() = 0;
    };
    
    // a Node is reducible and exposes the underlying type through a getter.
    template<typename T, typename Action>
    class Node : public Readable<T>, public Reducible<Action> {
    public:
      virtual ~Node() = default;
    
      virtual T get() = 0;
      virtual void reduce(Action) = 0;
    };
    
  }
    
}

#endif

#ifndef STATE_NODE_H
#define STATE_NODE_H

namespace State {

  template<typename A>
  class Reducible {
  public:
    virtual ~Reducible() = default;
    
    virtual void reduce(A) = 0;
  };
  
  template<typename T, typename A>
  class Node : public Reducible<A> {
  public:
    virtual ~Node() = default;
    
    virtual T value() = 0;
    virtual void reduce(A) = 0;
  };

}

#endif

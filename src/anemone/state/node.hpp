#ifndef STATE_NODE_H
#define STATE_NODE_H

namespace State {
  
  template<typename T, typename A>
  class Node {
  public:
    virtual ~Node() = default;

    virtual T value() = 0;
    virtual void reduce(A) = 0;
  };

}

#endif

#ifndef RX_OBSERVER_PREDICATE_H
#define RX_OBSERVER_PREDICATE_H


namespace rx {

  template<typename T>
  class Predicate {
  public:
    virtual ~Predicate() = default;
    virtual bool operator()(T) = 0;
  };

  template<typename T>
  class DefaultPredicate : public Predicate<T> {
  public:
    virtual bool operator()(T t) override { return true; };
  };
  

}

#endif

#ifndef RX_OBSERVER_PREDICATE_H
#define RX_OBSERVER_PREDICATE_H


namespace rx {
  
  template<typename T, typename S>
  class Filter {
  public:
    virtual ~Filter() = default;
    
    virtual S filter(T) = 0;        // filter from one type to another sub-type
    virtual bool predicate(S) = 0;  // evaluate predicate to determine whether we should broadcast
  };

  template<typename T>
  class DefaultFilter : public Filter<T, T> {
  public:
    virtual T filter(T t) override { return t; };
    virtual bool predicate(T t) override { return true; };
  };
  

}

#endif

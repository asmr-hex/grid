
template<typename T>
struct type_identity {
  using type = T;
};

template<typename T>
using reducer_fn_t = std::function<T(T, T)>;

template<typename T, typename... U>
using compose_fn_t = std::function<T(U...)>;


namespace State {
  
  template<typename T>
  class Node {
  public:
    virtual ~Node() = default;

    virtual T value() = 0;
    virtual void reduce(T) = 0;
  };


  template<typename T>
  class Leaf : public Node<T> {
  public:
    Leaf(const T& t, reducer_fn_t<T> reducer)
      : state(t), reducer(reducer) {};
  
    Leaf(T&& t, reducer_fn_t<T> reducer)
      : state(std::forward<T>(t)), reducer(std::forward<reducer_fn_t<T> >(reducer)) {};

    virtual T value() override {
      return state;
    }

    virtual void reduce(T t) override {
      state = reducer(state, t);
    };
  
  private:
    T state;
    reducer_fn_t<T> reducer;
  };


  template<typename T, typename...S>
  class Composition : public Node<T> {
  public:
    Composition(compose_fn_t<T, S...> composer, Node<S> *...s)
      // : nodes(s...)
    { curry_composer(composer, s...); };

    virtual T value() override {
      return compose();
    };

    virtual void reduce(T t) override {
      //
    };
    
  private:
    T state;
    
    // std::vector< std::variant<Node<S> *...> > nodes;
    std::function<T()> compose;

    void curry_composer(compose_fn_t<T, S...> fn, Node<S> *...s) {
      compose = [fn, s...] () -> T { return fn(s->value()...); };
    };
  };

  template<typename T>
  class State : public Node<T> {
  public:
    State(Node<T>&& node) : node(std::move(&node)) {};
    State(const T& t, reducer_fn_t<T> reducer)
      : node(new Leaf<T>(t, reducer)){
      // node = std::make_unique< Leaf<T> >(Leaf<T>(t, reducer));
    };
    
    State(T&& t, reducer_fn_t<T> reducer)
      : node(new Leaf<T>(std::forward<T>(t),
                         std::forward<reducer_fn_t<T> >(reducer)))
    {};

    template<typename... S>
    static State<T> compose(typename type_identity<compose_fn_t<T, S...> >::type fn,
                            Node<S> *...s)
    {
      return State<T>(Composition<T, S...>(fn, s...));
    };

    virtual T value() override {
      return node->value();
    }
    
    virtual void reduce(T t) override {
      //
    };
  
  protected:
   Node<T> *node;
  };

}

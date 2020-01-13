#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "anemone/rx/observer.hpp"


namespace ui {

  template<typename T>
  class Diff {
  public:
    T previous;
    T current;

    Diff(T t) : previous(t), current(t) {};
  };

  
  template<typename T, typename D>
  using derive_fn_t = std::function<D(T)>;

  
  class Component : public rx::Observer {
  public:
    Component(std::shared_ptr<State::Root> state, std::shared_ptr<Grid> grid) : state(state), grid(grid);
    virtual ~Component() = default;

    // all derived components must implement a connect function which is where they
    // should subscribe to the parts of the state tree they care about.
    virtual void connect() = 0;
    
    // all derived components must implement a render function which will be run
    // everytime the component must be re-rendered.
    virtual void render() = 0;

  protected:
    std::shared_ptr<Grid> grid;
    std::shared_ptr<State::Root> state;

    template<typename T, typename S, template D>
    void subscribe(rx::Observable<T> o, std::shared_ptr<Diff<D> > d, derive_fn_t<S, D> derive, rx::Filter<T, S> filter) {
      subscribe<T, S>(o, make_handler<S, D>(derive, d), filter);
    };

    template<typename T, template D>
    void subscribe(rx::Observable<T> o, std::shared_ptr<Diff<D> > d, derive_fn_t<T, D> fn, rx::Filter<T, T> filter) {
      subscribe<T>(o, make_handler<T, D>(derive, d), filter);
    };

    template<typename T, template D>
    void subscribe(rx::Observable<T> o, std::shared_ptr<Diff<D> > d, derive_fn_t<T, D> fn) {
      subscribe<T>(o, make_handler<T, D>(derive, d));
    };

  private:
    template<typename T, typename D>
    std::function<void(T)> make_handler(derive_fn_t<T, D> derive, std::shared_ptr<Diff<D> > d) {
      return [this, derive, d] (T t) -> void {
               // update previous and current states
               d->previous = d->current;
               d->current = derive(t);
               
               // only re-render if there has been a change in derived state
               if ( !(d->previous == d->current) )
                 render();
             };
    };
  };

}

#endif

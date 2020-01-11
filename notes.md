# high Frequency vs Low Frequency States
## generally, how to model these rapidly updated vs. slowly updated states?
the classification of these types of states as 'automated' vs 'user initiated' breaks down when we consider rapid midi input updates (e.g. from the midi glove or some other rapidly dispatching midi controller (maybe the knobs controller)). thus it might be more appropriate to make the distinction based on time-scale classes since the efficiency issues arise not from the regularity of the periodicity of the updates but rather the speed at which some of the updates occur. for example, with the midi glove, the input might come in highly irregular bursts but each burst will flood the midi innput with a lot of data very quickly. it is likely that the redux-style state tree wouldn't be able to stand up to such rapid bursts.

we should therefore describe the nature of the state as `high frequency` vs `low frequency` states

## clock driven state
since the small portion of the state which is driven by the clock undergoes rapid updates, it is inefficient to have clock-driven state updated via the redux-like state-reducer cycle. for this reason, it might be most efficient to break out the clock-driven state (we will call it the `?`) into its own self contained state which cann be efficiently updated on each tick cycle.

### what parts of the state are clock driven?
* `step.current` is literally the only state that rapidly updated by the clock.
* `part.page.in_playback` is also affected but less frequently, though it might be best to put this is clock-driven state since it is NOT a part of the state controlled by user interactions.

other parts of the state (e.g. parts of the state whose changes must be "synced" to the clock but are ultimately user invoked changes) should stay within the currently defined state!

## what about rapid midi inputs?
yes, these should also be handled seperately from the traditional redux-style state tree. having to process dense streams of midi in data would certainly cause cpu churn be issuing redux-actions to propogate through the entire state tree.

## requirements of the clock high frequency states
* processing of clock ticks must have access to Low Frequency State in order to determine if it needs to issue an action to sync a part of the Low Frequency State to the clock (e.g. `ppqn.current`, `ppqn.pending_change`, etc...). (alternatively, the issuer of the High Frequency State action *could* have 0 knowledge of the Low Frequency state and just issue a `part sequence has cycled` action once the `current step` is set back to 0, but this again will lead to a potentially high volume of periodic/rapid actions being sent to the Low Frequency State tree, thus is would be more optimal to only send those `part sequence has cycled` actions if we know we have to (e.g. `ppqn.pending_change == true` or `transport.is_about_to_pause == true`, etc!))
* it would be nice if the High Frequency State *had the same API* style as the Low Frequency State (i.e. used an action dispatcher). this should be simple.
* High Frequency States *must* be Observable! But, even moreso, observers must be able to subscribe and unsibscribe rapidly from these states. for example, the step UI only wants to subscribe to the current step of the currently rendered part if the step is on the currently rendered page! otherwise....WE DONT CARE!
* It would also be nice if the High Frequency States could be *nested* within the Low Frequency State tree! this way there is a self-contained and "single source of truth" state tree with a consistent, coherent, and unified API.

## Implementation ideas (satisfying above requirements)
### First of all, what would the High Frequency State look like on its own (not nested)?
* use actions: we want to be able to send actions to it to update it. we could use the match logic similar to what reducers use i think. in this case, each "reducer" function will just directly mutate the High Frequency State of interest rather than propogating the actions to substates. (AN ASSUMPTION HERE IS THAT THE VARIANT-STYLE "REDUCERS" (USING MATCH) ARE EFFICIENT ENOUGH TO HANDLE HIG FREQUENCY UPDATES...SHOULD BENCHMARK THIS). importantly, there should be *no* nested states in high frequency states! it should be a flat hierarchy since traversinng a dag for high frequency operations is costly!
``` c++

namespace high_frequency {
    namespace action {
        struct advance_step {};
    }

    using action_t = std::variant<action::advance_step>;
}

struct steps_t {
    int current = 0;
};

state = rx::HighFrequency::State<steps_t> // internaly setup steps_t state
      ::with_reducer<high_freq::action_t>
      ([] (steps_t state, high_freq::action_t action) {  // this function should return void!
          return match
          (action,
          [&] (const high_frequency::action::advance_step&) {
              state.current = state.current + 1;
          });
      });

```

* access to Low Frequency State: we need the processing of High Frequency State to have access to the Low Frequency State in order to make smart decisions about when to issue Low Frequency Actions as a result of High Frequency changes... we *could* include this logic inside the High Frequency "Reducer" functions which would make the logic more self-contained, but there are two reasons that this might be an anti-pattern
  1. redux principles say that reducers should never dispatch actions since this is the job of the controllers to dispatch actions
  2. including all dispatch logic within the controllers is more explicit w.r.t. business logic.
additionally, the typing templates might get a bit messy and could possibly cause cirular dependencies if we move ahead with trying to nest High Frequency States within the Low Frequency State tree. Therefore, it seems best to handle all subsequent Low Frequency Action dispatching resulting from High Frequency Actions within the controllers which are doing the dispatching.
* observable: we could easily reuse the observable pattern we already have for Low Frequency State nodes. However, since we would want some filtering (sinnce we aren't ALWAYS innterested in broadcasting updates) we need to flesh that out. One option is to pass in a predicate upon subscription! This predicate could be an object with its own state (possibly even subscribed to some part of the Low Frequency State!). In fact, maybe having this type of subscription predicate would be beneficial for even Low Frequency States!
Yes, this would be a worthwhile refactor...add an optional predicate to subscribers.
``` c++

class ObservablePredicate { // derived classes can also be Observers!
    virtual bool operator()() = 0; // use overloaded () operator!
}

class Observable<T> {
    ...
    void register_observer(Observable observer, ObservablePredicate pred) {

    }
}
```

* nested: okay...how to nest...so it would be nice if could just define our High Frequency States as Low Frequency State leaves! In fact, high Frequency States ARE leaves! so, this means composing must be aware of any High Frequency States which are being composed...
if the composition nodes are able to have a function which returns pointers to all High Frequency States, they can be bubbled up and aggregated onn state initialization up to the Root node. Then the Root node can implement one "reducer function" comprised of all the reducers of each High Frequency State (thus bypassing the "normal" Low Frequency State tree).


## Generally refactor rx::State stuff a little so we don't have to write so much boiler-plate code...
### big pain points currently
* having to define the `get` method for State wrappers
* defining custom equality operator!
* maybe invoke the match call internally within the library code!
* add a map composition type!

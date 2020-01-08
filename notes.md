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
* It would also be nice if the High Frequency States could be *nested* within the Low Frequency State tree!

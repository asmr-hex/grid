# UI
## how will it work?
* ui components will subscribe to events
* their subscription functions will set an internal variable called `component_must_rerender` if the subscribed state changes
*

## what kind of evennts will ui components need to subscribe to?
* state updates (so `rx::Observable`s)

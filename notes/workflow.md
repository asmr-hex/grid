# Workflow for adding new features
### Controller -> Action -> Reducer -> UI Cycle
these are the general steps one must take when introducing new features:
1. implement a `Controller` for the feature
  * subscribe it to the relevant io thing
  * add it to the controllers
2. implement an `ActionCreator` for generating the action to send to the reducers
  * implement the type of `action`
3. implement necessary `Selectors` if they dont exist already
4. implement branches of `Reducers` in state tree
5. implement `UI` component for the feature

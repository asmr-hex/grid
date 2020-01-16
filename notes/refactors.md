# List of Things to Refactor

### File/Directory Re-Organization
* for states, maybe don't have files named `state.hpp` etc...its not specific enough
* `types` : consolidate all relevant types

### Comments
* comments : start commenting all sections of code

### Renaming
* rename `State` to `Store`?

### Code Changes
* creating new states is still pretty onerous.... how can we change this for the better?
* move `dispatcher` into `rx` and make it a template!
* create header files for each "module" which will import all necessary stuff so we on;y have to import one header file instead of tonns of stuff from that "module"

#ifndef RX_REDUCER_H
#define RX_REDUCER_H

#include <memory>
#include <functional>


// reducer function type.
//
// this reducer type is specifically for low frequency state.
template<typename T, typename Action>
using reducer_fn_t = std::function<T(T, Action)>;


// high frequency reducer function type.
//
// this reducer type is specifically for high frequency states which are updated
// often and must mutate a pointer to data rather than returning a new copy of the
// data as in (most cases) with the low frequency state reducer.
template<typename T, typename Action>
using high_freq_reducer_fn_t = std::function<void(std::shared_ptr<T>, Action)>;


#endif

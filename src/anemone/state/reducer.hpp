#ifndef STATE_REDUCER_H
#define STATE_REDUCER_H

#include <functional>


template<typename T, typename A>
using reducer_fn_t = std::function<T(T, A)>;

#endif

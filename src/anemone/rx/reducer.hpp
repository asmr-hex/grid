#ifndef RX_REDUCER_H
#define RX_REDUCER_H

#include <functional>


template<typename T, typename Action>
using reducer_fn_t = std::function<T(T, Action)>;

#endif

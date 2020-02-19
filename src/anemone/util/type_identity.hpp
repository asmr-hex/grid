#ifndef UTIL_TYPE_IDENTITY_H
#define UTIL_TYPE_IDENTITY_H

template<typename T>
struct type_identity {
  using type = T;
};

#endif

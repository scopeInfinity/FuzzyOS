#pragma once

// This is internal headerfile to be included by utility.h only

#include <utility.h>

namespace std {

template <typename T1, typename T2> pair<T1, T2>::pair() {}

template <typename T1, typename T2>
pair<T1, T2>::pair(const T1 &first, const T2 &second)
    : first(first), second(second) {}

} // namespace std

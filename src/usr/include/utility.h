#pragma once

namespace std {

template <typename T1, typename T2> class pair {
  public:
    T1 first;
    T2 second;
    pair();
    pair(const T1 &first, const T2 &second);
};

template <typename T1, typename T2>
inline pair<T1, T2> make_pair(const T1 &a, const T2 &b) {
    return pair<T1, T2>(a, b);
}

} // namespace std

#include <utility.tcc>

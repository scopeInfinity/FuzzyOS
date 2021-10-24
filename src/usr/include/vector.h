#pragma once

#include <stddef.h>

namespace std {

template <typename T> class vector {
    std::size_t _capacity, _size;
    T *_data;

  public:
    typedef T *iterator;
    typedef const T *const_iterator;

    vector();
    vector(std::size_t size);
    vector(std::size_t size, const T &_default);
    vector(const vector<T> &o);
    ~vector();

    vector &operator=(const vector<T> &o);

    void reserve(std::size_t n);
    void resize(std::size_t new_size);
    void resize(std::size_t new_size, const T &_default);
    void push_back(const T &val);
    void pop_back();

    void clear();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    bool empty() const;
    std::size_t size() const;

    T &back();
    T &front();
    T &at(std::size_t pos);
    T &operator[](std::size_t pos);
    const T &operator[](std::size_t pos) const;
};

} // namespace std

template <typename T>
inline bool operator!=(const std::vector<T> &a, const std::vector<T> b) {
    if (a.size() != b.size())
        return true;
    for (std::size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return true;
    }
    return false;
}

template <typename T>
inline bool operator==(const std::vector<T> &a, const std::vector<T> b) {
    return !(a != b);
}

#include <vector.tcc>
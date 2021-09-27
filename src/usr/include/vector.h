#pragma once

#include <stddef.h>

namespace std {

template<typename T>
class vector {
    std::size_t _capacity, _size;
    T *_data;
    void resize_capacity(std::size_t capacity);

public:
    typedef T* iterator;
    typedef const T* const_iterator;

    vector();
    vector(std::size_t size, const T &_default);
    vector(const vector<T> &o);
    vector& operator=(const vector<T> &o);
    ~vector();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    void clear();
    bool empty() const;
    std::size_t size() const;
    void push_back(const T &val);
    void pop_back();

    T& back();
    T& front();
    T& at(std::size_t pos);
    T& operator[](std::size_t pos);
    const T& operator[](std::size_t pos) const;
};

}  // namespace std end

#include <vector.tcc>

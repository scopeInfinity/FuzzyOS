#pragma once

#include <stddef.h>

namespace std {

template<typename T>
class vector {
    std::size_t _capacity, _size;
    T *_data;
    void resize_capacity(std::size_t capacity);

public:
    vector<T>();
    vector<T>(std::size_t size, const T &_default);

    T *raw_data() const;
    bool empty();
    std::size_t size();
    void push_back(const T &val);
    void pop_back();

    T& back();
    T& front();
    T& at(std::size_t pos);
    T& operator[](std::size_t pos);
};

}  // namespace std end

#include <vector.tcc>

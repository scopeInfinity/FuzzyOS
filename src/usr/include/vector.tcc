#pragma once

// This is internal headerfile to be included by vector.h only

#include <vector.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>

namespace std {

static std::size_t next_power2(std::size_t size) {
    // assumes no overflow
    std::size_t p = 1;
    while (p && p < size) p <<= 1;
    return p;
}

template <typename T>
vector<T>::vector() : _data(NULL), _size(0) {
    resize_capacity(1);
}

template <typename T>
vector<T>::vector(std::size_t size, const T &_default): _size(size)  {
    this->_data = NULL;
    resize_capacity(size);
    for(std::size_t i = 0; i < size; i++) {
        this->_data[i] = _default;
    }
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() { return this->_data; }

template <typename T>
typename vector<T>::iterator vector<T>::end() { return this->_data + this->_size; }

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const { return this->_data; }

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const { return this->_data + this->_size; }

template <typename T>
void vector<T>::resize_capacity(std::size_t capacity) {
    // internal; assumes capacity to be power of 2
    // and will also make a copy of array and move _data pointer.
    // assumes size<=current_capacity and size<=new_capacity
    const std::size_t data_size = capacity*sizeof(T);
    T *_new_data = (T*) std::malloc(data_size);
    if (!_new_data) return;  // malloc failed
    std::memcpy(_new_data, this->_data, data_size);
    std::free(this->_data);  // should be no-op if _data == NULL
    this->_data = _new_data;
    this->_capacity = capacity;
}

template <typename T>
bool vector<T>::empty() {
    return this->_size == 0;
}

template <typename T>
std::size_t vector<T>::size() {
    return this->_size;
}

template <typename T>
void vector<T>::push_back(const T &val) {
    if(this->_size == this->_capacity) {
        resize_capacity(this->_capacity*2);
    }
    this->_data[this->_size++] = val;
}

template <typename T>
void vector<T>::pop_back() {
    // not reducing the capacity for now
    if(this->_size > 0) {
        this->_size--;
    }
}

template <typename T>
T &vector<T>::back() {
    return this->_data[this->_size-1];
}

template <typename T>
T &vector<T>::front() {
    return this->_data[0];
}

template <typename T>
T &vector<T>::at(std::size_t pos) {
    return this->_data[pos];
}

template <typename T>
T &vector<T>::operator[](std::size_t pos) {
    return this->_data[pos];
}

} // namespace std end

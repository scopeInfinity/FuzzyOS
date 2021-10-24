#pragma once

// This is internal headerfile to be included by vector.h only

#include <new.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector.h>

namespace std {

template <typename T>
vector<T>::vector() : _data(NULL), _capacity(0), _size(0) {}

template <typename T>
vector<T>::vector(std::size_t size) : _data(NULL), _capacity(0), _size(0) {
    reserve(size);
    // initializing the members
    for (std::size_t i = 0; i < size; i++) {
        new (_data + i) T(); // default constructor
    }
    _size = size;
}

template <typename T>
vector<T>::vector(std::size_t size, const T &_default)
    : _data(NULL), _capacity(0), _size(0) {
    reserve(size);
    // initializing the members
    for (std::size_t i = 0; i < size; i++) {
        new (_data + i) T(_default); // copy constructor
    }
    _size = size;
}

template <typename T>
vector<T>::vector(const vector<T> &o) : _data(NULL), _capacity(0), _size(0) {
    reserve(o._size);
    // initializing the members
    for (std::size_t i = 0; i < o._size; i++) {
        new (_data + i) T(o._data[i]); // copy constructor
    }
    _size = o._size;
}

template <typename T> vector<T> &vector<T>::operator=(const vector<T> &o) {
    resize(o._size);
    // re-initialize the members
    for (std::size_t i = 0; i < o._size; i++) {
        _data[i] = o._data[i]; // assignment operator
    }
    return *this;
}

template <typename T> vector<T>::~vector() {
    clear();
    reserve(0);
}

template <typename T> void vector<T>::reserve(std::size_t new_capacity) {
    if (new_capacity < _size)
        new_capacity = _size; // never reserve less than size
    if (new_capacity == _capacity)
        return; // resize not needed

    const std::size_t new_capacity_size = (new_capacity) * sizeof(T);

    // make a copy of array and move _data pointer.
    _data = (T *)std::realloc(_data, new_capacity_size);
    _capacity = new_capacity;
}

template <typename T> void vector<T>::resize(const std::size_t new_size) {
    if (_size < new_size) {
        reserve(new_size);
        for (std::size_t i = _size; i < new_size; i++) {
            new (_data + i) T(); // default constructor
        }
        _size = new_size;
    } else {
        for (std::size_t i = new_size; i < _size; i++) {
            _data[i].~T(); // explicit destructor call
        }
        _size = new_size;
        // not reducing the allocated capacity.
    }
}

template <typename T>
void vector<T>::resize(const std::size_t new_size, const T &_default) {
    if (_size < new_size) {
        reserve(new_size);
        for (std::size_t i = _size; i < new_size; i++) {
            new (_data + i) T(_default); // copy constructor
        }
        _size = new_size;
    } else {
        for (std::size_t i = new_size; i < _size; i++) {
            _data[i].~T(); // explicit destructor call
        }
        _size = new_size;
        // not reducing the allocated capacity.
    }
}

template <typename T> void vector<T>::push_back(const T &val) {
    if (_size == _capacity) {
        reserve(max(1, _capacity * 2));
    }
    new (_data + (_size++)) T(val); // copy constructor
}

template <typename T> void vector<T>::pop_back() {
    if (!(_size > 0)) {
        return;
    }

    _size--;
    _data[_size].~T(); // explicit destructor call

    // not reducing the allocated capacity.
}

template <typename T> void vector<T>::clear() { resize(0); }

template <typename T> typename vector<T>::iterator vector<T>::begin() {
    return _data;
}

template <typename T> typename vector<T>::iterator vector<T>::end() {
    return _data + _size;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
    return _data;
}

template <typename T>
typename vector<T>::const_iterator vector<T>::end() const {
    return _data + _size;
}

template <typename T> bool vector<T>::empty() const { return _size == 0; }

template <typename T> std::size_t vector<T>::size() const { return _size; }

template <typename T> T &vector<T>::back() { return _data[_size - 1]; }

template <typename T> T &vector<T>::front() { return _data[0]; }

template <typename T> T &vector<T>::at(std::size_t pos) { return _data[pos]; }

template <typename T> T &vector<T>::operator[](std::size_t pos) {
    return _data[pos];
}

template <typename T> const T &vector<T>::operator[](std::size_t pos) const {
    return _data[pos];
}

} // namespace std
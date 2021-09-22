#pragma once

// This is internal headerfile to be included by string.h only

#include <string.h>

namespace std {

template <typename CharT>
basic_string<CharT>::basic_string() : _data(1, '\0') {}

template <typename CharT>
basic_string<CharT>::basic_string(std::size_t n, CharT c) : _data(n+1, c) {
    this->_data[n] = '\0';
}

template <typename CharT>
basic_string<CharT>::basic_string(const CharT* str) : _data(std::strlen(str)+1, '\0') {
    for (std::size_t i = 0; i < this->_data.size(); i++) {
        this->_data[i] = str[i];
    }
}

template <typename CharT>
basic_string<CharT>::basic_string(const CharT* str, std::size_t n) {
    int str_n = std::strlen(str);
    this->_data(n+1, '\0');
    for (std::size_t i = 0; i < n && i < str_n; i++) {
        this->_data[i] = str[i];
    }
}

template <typename CharT>
CharT& basic_string<CharT>::back() {
    return this->_data.at(this->_data.size()-2);
}

template <typename CharT>
CharT& basic_string<CharT>::front() {
    return this->_data.at(0);
}

template <typename CharT>
CharT& basic_string<CharT>::at(std::size_t pos) {
    return this->_data.at(pos);
}

template <typename CharT>
CharT& basic_string<CharT>::operator[](std::size_t pos) {
    return this->_data[pos];
}

template <typename CharT>
const CharT *basic_string<CharT>::c_str() const {
    return this->_data.raw_data();
}

} // namespace std end

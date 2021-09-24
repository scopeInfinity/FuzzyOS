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
basic_string<CharT>::basic_string(const CharT* str, std::size_t n) : _data(n+1, '\0') {
    int str_n = std::strlen(str);
    for (std::size_t i = 0; i < n && i < str_n; i++) {
        this->_data[i] = str[i];
    }
}

template <typename CharT>
typename basic_string<CharT>::iterator basic_string<CharT>::begin() { return this->_data.begin(); }

template <typename CharT>
typename basic_string<CharT>::iterator basic_string<CharT>::end() { return this->_data.end() - 1; }

template <typename CharT>
typename basic_string<CharT>::const_iterator basic_string<CharT>::begin() const { return this->_data.begin(); }

template <typename CharT>
typename basic_string<CharT>::const_iterator basic_string<CharT>::end() const { return this->_data.end() - 1; }

template <typename CharT>
void basic_string<CharT>::clear() {
    this->_data.clear();
    this->_data.push_back('\0');
}

template <typename CharT>
void basic_string<CharT>::pop_back() {
    if(this->_data.size()>1) {
        this->_data.pop_back();
        this->_data.last() = '\0';
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
    return this->_data.begin();
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator+=(const CharT c) {
    this->_data.back() = c;  // replace null character
    this->_data.push_back('\0');  // add null character
    return *this;
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator+=(const CharT* o) {
    this->_data.pop_back();  // remove null character
    while ((*o))  {
        this->_data.push_back(*(o++));
    }
    this->_data.push_back('\0');  // add null character
    return *this;
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator+=(const basic_string<CharT> &o) {
    this->_data.pop_back();  // remove null character
    for(const CharT &c: o) {
        this->_data.push_back(c);
    }
    this->_data.push_back('\0');  // add null character
    return *this;
}

std::istream& get_line(std::istream &i, std::string &str) {
    char ch;
    str.clear();
    while (1) {
        i.get(ch);
        if (ch == '\n') break;
        str += ch;
    }
}

} // namespace std end

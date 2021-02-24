#include "my_vector.h"

#include <algorithm>

std::size_t MyVector::size() const {
    return _sz;
}

std::size_t MyVector::capacity() const {
    return _cp;
}

MyVector::MyVector(std::size_t init_capacity) {
    reserve(init_capacity);
}

MyVector::MyVector() : MyVector(2) {}

MyVector::~MyVector() {
    delete[] _data;
    _data = nullptr;
    _cp = _sz = 0;
}

MyVector::MyVector(const MyVector &other) {
    reserve(other.size());
    std::copy_n(other._data, other.size(), _data);
    _sz = other.size();
}

MyVector::MyVector(MyVector &&other) noexcept {
    swap(other);
}

MyVector &MyVector::operator=(const MyVector &other) {
    if (&other == this)
        return *this;

    reserve(other.size());
    std::copy_n(other._data, other.size(), _data);
    _sz = other.size();
    return *this;
}

MyVector &MyVector::operator=(MyVector &&other) noexcept {
    swap(other);
    return *this;
}

void MyVector::swap(MyVector &other) {
    std::swap(_data, other._data);
    std::swap(_cp, other._cp);
    std::swap(_sz, other._sz);
}

void MyVector::set(std::size_t index, int value) {
    _data[index] = value;
}

int MyVector::get(std::size_t index) const {
    return _data[index];
}

void MyVector::reserve(std::size_t new_capacity) {
    if (new_capacity <= _cp)
        return;

    int *new_data = new int[new_capacity];
    std::copy_n(_data, _sz, new_data);

    delete[] _data;
    _data = new_data;
    _cp = new_capacity;
}

void MyVector::resize(std::size_t new_size) {
    reserve(std::max(new_size, 2 * _cp));
    if (new_size > _sz)
        std::fill(_data + _sz, _data + new_size, 0);
    _sz = new_size;
}

void MyVector::push_back(int value) {
    if (_sz == _cp)
        reserve(std::max<std::size_t>(1, 2 * _cp));
    _data[_sz++] = value;
}

void MyVector::insert(std::size_t index, int value) {
    if (_sz == _cp)
        reserve(std::max<std::size_t>(1, 2 * _cp));
    memmove(_data + index + 1, _data + index, (_sz - index) * sizeof(int));
    _data[index] = value;
    ++_sz;
}

void MyVector::erase(std::size_t index) {
    memmove(_data + index, _data + index + 1, (_sz - index - 1) * sizeof(int));
    --_sz;
}

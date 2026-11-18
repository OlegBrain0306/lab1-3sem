#include "array.h"

Array::Array() : data(nullptr), size(0), capacity(0) {}

Array::~Array() {
    delete[] data;
}

// Увеличение ёмкости в 2 раза с копированием элементов: O(n),
// но происходит редко, поэтому push в среднем O(1)
void Array::grow() {
    int newCapacity = (capacity == 0) ? 4 : capacity * 2;
    std::string* newData = new std::string[newCapacity];
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void Array::push(const std::string& value) {
    if (size == capacity) {
        grow();
    }
    data[size] = value;
    size++;
}

// Вставка по индексу: элементы справа сдвигаются на одну позицию, O(n)
bool Array::insert(int index, const std::string& value) {
    if (index < 0 || index > size) {
        return false;
    }
    if (size == capacity) {
        grow();
    }
    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
    return true;
}

bool Array::get(int index, std::string& out) const {
    if (index < 0 || index >= size) {
        return false;
    }
    out = data[index];
    return true;
}

bool Array::set(int index, const std::string& value) {
    if (index < 0 || index >= size) {
        return false;
    }
    data[index] = value;
    return true;
}

// Удаление по индексу: элементы справа сдвигаются влево, O(n)
bool Array::remove(int index, std::string& removed) {
    if (index < 0 || index >= size) {
        return false;
    }
    removed = data[index];
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }
    size--;
    return true;
}

int Array::length() const {
    return size;
}

std::string Array::toString(const std::string& sep) const {
    std::string result;
    for (int i = 0; i < size; i++) {
        if (i > 0) {
            result += sep;
        }
        result += data[i];
    }
    return result;
}

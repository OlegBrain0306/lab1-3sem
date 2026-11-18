#pragma once

#include <string>

// Динамический массив строк.
// Элементы лежат в памяти подряд; при нехватке места ёмкость удваивается.
class Array {
public:
    Array();
    ~Array();
    Array(const Array&) = delete;            // копирование запрещено, чтобы
    Array& operator=(const Array&) = delete; // не освободить память дважды

    void push(const std::string& value);                     // в конец
    bool insert(int index, const std::string& value);        // по индексу, 0..size
    bool get(int index, std::string& out) const;             // чтение по индексу
    bool set(int index, const std::string& value);           // замена по индексу
    bool remove(int index, std::string& removed);            // удаление по индексу
    int length() const;
    std::string toString(const std::string& sep) const;      // чтение всего массива

private:
    std::string* data;
    int size;
    int capacity;

    void grow();
};

#pragma once

#include <string>

// Стек (LIFO) на односвязном списке: вершина — голова списка.
class Stack {
public:
    Stack();
    ~Stack();
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const std::string& value);
    bool pop(std::string& removed);      // чтение и удаление вершины
    bool peek(std::string& out) const;   // чтение вершины без удаления
    int length() const;
    std::string toString(const std::string& sep) const; // от дна к вершине

private:
    struct Node {
        std::string data;
        Node* next; // элемент под этим
    };
    Node* top;
    int size;

    static std::string fromBottom(const Node* node, const std::string& sep);
};

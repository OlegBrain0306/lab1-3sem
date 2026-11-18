#pragma once

#include <string>

// Очередь (FIFO) на односвязном списке: добавляем в хвост, забираем из головы.
class Queue {
public:
    Queue();
    ~Queue();
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void push(const std::string& value);
    bool pop(std::string& removed);      // чтение и удаление головы
    bool peek(std::string& out) const;   // чтение головы без удаления
    int length() const;
    std::string toString(const std::string& sep) const; // от головы к хвосту

private:
    struct Node {
        std::string data;
        Node* next;
    };
    Node* head;
    Node* tail;
    int size;
};

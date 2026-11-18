#include "queue.h"

Queue::Queue() : head(nullptr), tail(nullptr), size(0) {}

Queue::~Queue() {
    std::string tmp;
    while (pop(tmp)) {
    }
}

void Queue::push(const std::string& value) {
    Node* node = new Node{value, nullptr};
    if (tail == nullptr) {
        head = node;
    } else {
        tail->next = node;
    }
    tail = node;
    size++;
}

bool Queue::pop(std::string& removed) {
    if (head == nullptr) {
        return false;
    }
    Node* old = head;
    removed = old->data;
    head = old->next;
    if (head == nullptr) {
        tail = nullptr;
    }
    delete old;
    size--;
    return true;
}

bool Queue::peek(std::string& out) const {
    if (head == nullptr) {
        return false;
    }
    out = head->data;
    return true;
}

int Queue::length() const {
    return size;
}

std::string Queue::toString(const std::string& sep) const {
    std::string result;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur != head) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

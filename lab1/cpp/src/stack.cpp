#include "stack.h"

Stack::Stack() : top(nullptr), size(0) {}

Stack::~Stack() {
    std::string tmp;
    while (pop(tmp)) {
    }
}

void Stack::push(const std::string& value) {
    top = new Node{value, top};
    size++;
}

bool Stack::pop(std::string& removed) {
    if (top == nullptr) {
        return false;
    }
    Node* old = top;
    removed = old->data;
    top = old->next;
    delete old;
    size--;
    return true;
}

bool Stack::peek(std::string& out) const {
    if (top == nullptr) {
        return false;
    }
    out = top->data;
    return true;
}

int Stack::length() const {
    return size;
}

// Список идёт от вершины вниз, а выводим от дна: сначала рекурсивно нижние элементы
std::string Stack::fromBottom(const Node* node, const std::string& sep) {
    if (node == nullptr) {
        return "";
    }
    std::string below = fromBottom(node->next, sep);
    return below.empty() ? node->data : below + sep + node->data;
}

std::string Stack::toString(const std::string& sep) const {
    return fromBottom(top, sep);
}

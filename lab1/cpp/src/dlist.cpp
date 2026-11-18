#include "dlist.h"

DList::DList() : head(nullptr), tail(nullptr), size(0) {}

DList::~DList() {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

DList::Node* DList::findNode(const std::string& value) const {
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->data == value) {
            return cur;
        }
    }
    return nullptr;
}

// Узел знает обоих соседей, поэтому удаление из любого места — O(1)
void DList::unlink(Node* node) {
    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        head = node->next;
    }
    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        tail = node->prev;
    }
    delete node;
    size--;
}

void DList::pushHead(const std::string& value) {
    Node* node = new Node{value, nullptr, head};
    if (head != nullptr) {
        head->prev = node;
    } else {
        tail = node;
    }
    head = node;
    size++;
}

void DList::pushTail(const std::string& value) {
    Node* node = new Node{value, tail, nullptr};
    if (tail != nullptr) {
        tail->next = node;
    } else {
        head = node;
    }
    tail = node;
    size++;
}

bool DList::pushBefore(const std::string& target, const std::string& value) {
    Node* cur = findNode(target);
    if (cur == nullptr) {
        return false;
    }
    if (cur == head) {
        pushHead(value);
        return true;
    }
    Node* node = new Node{value, cur->prev, cur};
    cur->prev->next = node;
    cur->prev = node;
    size++;
    return true;
}

bool DList::pushAfter(const std::string& target, const std::string& value) {
    Node* cur = findNode(target);
    if (cur == nullptr) {
        return false;
    }
    if (cur == tail) {
        pushTail(value);
        return true;
    }
    Node* node = new Node{value, cur, cur->next};
    cur->next->prev = node;
    cur->next = node;
    size++;
    return true;
}

bool DList::delHead(std::string& removed) {
    if (head == nullptr) {
        return false;
    }
    removed = head->data;
    unlink(head);
    return true;
}

// В отличие от односвязного списка — O(1), т.к. есть tail->prev
bool DList::delTail(std::string& removed) {
    if (tail == nullptr) {
        return false;
    }
    removed = tail->data;
    unlink(tail);
    return true;
}

bool DList::delBefore(const std::string& target, std::string& removed) {
    Node* cur = findNode(target);
    if (cur == nullptr || cur->prev == nullptr) {
        return false;
    }
    removed = cur->prev->data;
    unlink(cur->prev);
    return true;
}

bool DList::delAfter(const std::string& target, std::string& removed) {
    Node* cur = findNode(target);
    if (cur == nullptr || cur->next == nullptr) {
        return false;
    }
    removed = cur->next->data;
    unlink(cur->next);
    return true;
}

bool DList::delValue(const std::string& value) {
    Node* cur = findNode(value);
    if (cur == nullptr) {
        return false;
    }
    unlink(cur);
    return true;
}

bool DList::find(const std::string& value) const {
    return findNode(value) != nullptr;
}

// Идём с той стороны, которая ближе к индексу
bool DList::get(int index, std::string& out) const {
    if (index < 0 || index >= size) {
        return false;
    }
    Node* cur;
    if (index < size / 2) {
        cur = head;
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
    } else {
        cur = tail;
        for (int i = size - 1; i > index; i--) {
            cur = cur->prev;
        }
    }
    out = cur->data;
    return true;
}

int DList::length() const {
    return size;
}

std::string DList::toString(const std::string& sep) const {
    std::string result;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur != head) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

std::string DList::toStringReverse(const std::string& sep) const {
    std::string result;
    for (Node* cur = tail; cur != nullptr; cur = cur->prev) {
        if (cur != tail) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

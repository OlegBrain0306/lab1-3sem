#include "flist.h"

FList::FList() : head(nullptr), tail(nullptr), size(0) {}

FList::~FList() {
    while (head != nullptr) {
        Node* next = head->next;
        delete head;
        head = next;
    }
}

void FList::pushHead(const std::string& value) {
    Node* node = new Node{value, head};
    head = node;
    if (tail == nullptr) {
        tail = node;
    }
    size++;
}

void FList::pushTail(const std::string& value) {
    Node* node = new Node{value, nullptr};
    if (tail == nullptr) {
        head = node;
    } else {
        tail->next = node;
    }
    tail = node;
    size++;
}

// Вставка перед target: нужен предыдущий узел, поэтому идём от головы, O(n)
bool FList::pushBefore(const std::string& target, const std::string& value) {
    if (head == nullptr) {
        return false;
    }
    if (head->data == target) {
        pushHead(value);
        return true;
    }
    for (Node* cur = head; cur->next != nullptr; cur = cur->next) {
        if (cur->next->data == target) {
            cur->next = new Node{value, cur->next};
            size++;
            return true;
        }
    }
    return false;
}

bool FList::pushAfter(const std::string& target, const std::string& value) {
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->data == target) {
            cur->next = new Node{value, cur->next};
            if (cur == tail) {
                tail = cur->next;
            }
            size++;
            return true;
        }
    }
    return false;
}

bool FList::delHead(std::string& removed) {
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

// Удаление хвоста: предыдущий узел неизвестен, ищем его от головы, O(n)
bool FList::delTail(std::string& removed) {
    if (head == nullptr) {
        return false;
    }
    if (head == tail) {
        return delHead(removed);
    }
    Node* cur = head;
    while (cur->next != tail) {
        cur = cur->next;
    }
    removed = tail->data;
    delete tail;
    tail = cur;
    tail->next = nullptr;
    size--;
    return true;
}

// Удаление узла перед target
bool FList::delBefore(const std::string& target, std::string& removed) {
    if (head == nullptr || head->data == target) {
        return false; // перед головой ничего нет
    }
    if (head->next != nullptr && head->next->data == target) {
        return delHead(removed);
    }
    // ищем prev, у которого prev->next->next == target
    for (Node* prev = head; prev->next != nullptr && prev->next->next != nullptr; prev = prev->next) {
        if (prev->next->next->data == target) {
            Node* victim = prev->next;
            removed = victim->data;
            prev->next = victim->next;
            delete victim;
            size--;
            return true;
        }
    }
    return false;
}

bool FList::delAfter(const std::string& target, std::string& removed) {
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->data == target) {
            Node* victim = cur->next;
            if (victim == nullptr) {
                return false; // после хвоста ничего нет
            }
            removed = victim->data;
            cur->next = victim->next;
            if (victim == tail) {
                tail = cur;
            }
            delete victim;
            size--;
            return true;
        }
    }
    return false;
}

// Удаление первого узла с данным значением
bool FList::delValue(const std::string& value) {
    std::string removed;
    if (head == nullptr) {
        return false;
    }
    if (head->data == value) {
        return delHead(removed);
    }
    for (Node* prev = head; prev->next != nullptr; prev = prev->next) {
        if (prev->next->data == value) {
            Node* victim = prev->next;
            prev->next = victim->next;
            if (victim == tail) {
                tail = prev;
            }
            delete victim;
            size--;
            return true;
        }
    }
    return false;
}

bool FList::find(const std::string& value) const {
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur->data == value) {
            return true;
        }
    }
    return false;
}

bool FList::get(int index, std::string& out) const {
    if (index < 0 || index >= size) {
        return false;
    }
    Node* cur = head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    out = cur->data;
    return true;
}

int FList::length() const {
    return size;
}

std::string FList::toString(const std::string& sep) const {
    std::string result;
    for (Node* cur = head; cur != nullptr; cur = cur->next) {
        if (cur != head) {
            result += sep;
        }
        result += cur->data;
    }
    return result;
}

// В односвязном списке нет ссылок назад, поэтому обратный вывод делаем рекурсией:
// сначала выводим остаток списка, затем текущий узел
std::string FList::reverseFrom(const Node* node, const std::string& sep) {
    if (node == nullptr) {
        return "";
    }
    std::string rest = reverseFrom(node->next, sep);
    return rest.empty() ? node->data : rest + sep + node->data;
}

std::string FList::toStringReverse(const std::string& sep) const {
    return reverseFrom(head, sep);
}

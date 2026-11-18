#pragma once

#include <string>

// Двусвязный список строк: у каждого узла ссылки на следующий и предыдущий.
class DList {
public:
    DList();
    ~DList();
    DList(const DList&) = delete;
    DList& operator=(const DList&) = delete;

    // Добавление (4 способа)
    void pushHead(const std::string& value);
    void pushTail(const std::string& value);
    bool pushBefore(const std::string& target, const std::string& value);
    bool pushAfter(const std::string& target, const std::string& value);

    // Удаление (4 способа + по значению)
    bool delHead(std::string& removed);
    bool delTail(std::string& removed);
    bool delBefore(const std::string& target, std::string& removed);
    bool delAfter(const std::string& target, std::string& removed);
    bool delValue(const std::string& value);

    // Поиск и чтение
    bool find(const std::string& value) const;
    bool get(int index, std::string& out) const;
    int length() const;
    std::string toString(const std::string& sep) const;        // голова -> хвост
    std::string toStringReverse(const std::string& sep) const; // хвост -> голова

private:
    struct Node {
        std::string data;
        Node* prev;
        Node* next;
    };
    Node* head;
    Node* tail;
    int size;

    Node* findNode(const std::string& value) const;
    void unlink(Node* node); // вырезать узел из списка и удалить его, O(1)
};

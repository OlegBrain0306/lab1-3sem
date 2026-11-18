#include "fbt.h"

#include <string>

namespace {
// Небольшая очередь указателей для работы с файлом и вставки по уровням
template <typename T>
struct PtrQueue {
    T** data;
    int head = 0, tail = 0;

    explicit PtrQueue(int n) : data(new T*[n]) {}
    ~PtrQueue() { delete[] data; }
    PtrQueue(const PtrQueue&) = delete;
    PtrQueue& operator=(const PtrQueue&) = delete;

    void push(T* node) { data[tail++] = node; }
    T* pop() { return data[head++]; }
    bool empty() const { return head == tail; }
};

void appendWord(std::string& out, const std::string& word) {
    if (!out.empty()) out += ' ';
    out += word;
}

bool parseInt(const std::string& s, int& value) {
    try {
        std::size_t pos = 0;
        value = std::stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}
} // namespace

FullBinaryTree::FullBinaryTree() : root(nullptr), count(0) {}
FullBinaryTree::~FullBinaryTree() { clear(root); }

// Удаляем потомков, затем сам узел.
void FullBinaryTree::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

// Поиск прямым рекурсивным обходом: корень -> левый -> правый.
FullBinaryTree::Node* FullBinaryTree::find(Node* node, int value) {
    if (!node || node->data == value) return node;
    Node* p = find(node->left, value);
    return p ? p : find(node->right, value);
}

bool FullBinaryTree::contains(int value) const {
    return find(root, value) != nullptr;
}

// Обычная вставка: ищем свободного потомка по уровням.
FullBinaryTree::Result FullBinaryTree::insert(int value) {
    if (contains(value)) return EXISTS;
    if (!root) {
        root = new Node{value, nullptr, nullptr};
        count = 1;
        return OK;
    }
    PtrQueue<Node> q(count);
    q.push(root);
    while (!q.empty()) {
        Node* p = q.pop();
        if (!p->left || !p->right) {
            Node*& slot = p->left ? p->right : p->left;
            slot = new Node{value, nullptr, nullptr};
            count++;
            return OK;
        }
        q.push(p->left);
        q.push(p->right);
    }
    return OK;
}

FullBinaryTree::Result FullBinaryTree::insertAt(int parent, char side, int value) {
    Node* p = find(root, parent);
    if (!p) return NO_PARENT;
    if (contains(value)) return EXISTS;
    Node*& child = (side == 'L') ? p->left : p->right;
    if (child) return OCCUPIED;
    child = new Node{value, nullptr, nullptr};
    count++;
    return OK;
}

// Добавление двух потомков сразу не нарушает свойство Full
FullBinaryTree::Result FullBinaryTree::insertPair(int parent, int left, int right) {
    if (left == right || left == parent || right == parent) return EXISTS;
    if (!root) {
        root = new Node{parent, nullptr, nullptr};
        count = 1;
    }
    Node* p = find(root, parent);
    if (!p) return NO_PARENT;
    if (p->left || p->right) return NOT_LEAF;
    if (contains(left) || contains(right)) return EXISTS;
    p->left = new Node{left, nullptr, nullptr};
    p->right = new Node{right, nullptr, nullptr};
    count += 2;
    return OK;
}

bool FullBinaryTree::full(Node* node) {
    if (!node) return true;
    if ((node->left == nullptr) != (node->right == nullptr)) return false;
    return full(node->left) && full(node->right);
}

bool FullBinaryTree::isFull() const { return full(root); }

void FullBinaryTree::preorderFrom(Node* node, std::string& out) {
    if (!node) return;
    appendWord(out, std::to_string(node->data));
    preorderFrom(node->left, out);
    preorderFrom(node->right, out);
}

std::string FullBinaryTree::preorder() const {
    std::string out;
    preorderFrom(root, out);
    return out;
}

// Обход в ширину: выводим узлы по уровням с помощью очереди FIFO
std::string FullBinaryTree::bfs() const {
    std::string out;
    if (!root) return out;
    PtrQueue<Node> q(count);
    q.push(root);
    while (!q.empty()) {
        Node* node = q.pop();
        appendWord(out, std::to_string(node->data));
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
    return out;
}

// Формат файла сохраняем прежним: по уровням, # обозначает пропуск
std::string FullBinaryTree::serialize() const {
    if (!root) return "";
    std::string out = std::to_string(root->data);
    std::string skipped;
    PtrQueue<Node> q(count);
    q.push(root);
    while (!q.empty()) {
        Node* p = q.pop();
        Node* children[] = {p->left, p->right};
        for (Node* child : children) {
            if (!child) {
                skipped += " #";
            } else {
                out += skipped + " " + std::to_string(child->data);
                skipped.clear();
                q.push(child);
            }
        }
    }
    return out;
}

bool FullBinaryTree::load(const Array& items) {
    clear(root);
    root = nullptr;
    count = 0;
    if (items.length() == 0) return true;

    std::string s;
    int value;
    items.get(0, s);
    if (!parseInt(s, value)) return false;
    root = new Node{value, nullptr, nullptr};
    count = 1;

    PtrQueue<Node> q(items.length());
    q.push(root);
    int i = 1;
    while (!q.empty() && i < items.length()) {
        Node* p = q.pop();
        for (int side = 0; side < 2 && i < items.length(); side++, i++) {
            items.get(i, s);
            if (s == "#") continue;
            if (!parseInt(s, value) || contains(value)) return false;
            Node* child = new Node{value, nullptr, nullptr};
            if (side == 0) p->left = child;
            else p->right = child;
            q.push(child);
            count++;
        }
    }
    return i == items.length();
}

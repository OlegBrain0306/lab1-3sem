#pragma once

#include <string>
#include "array.h"

// Full Binary Tree: у каждого узла либо 0, либо 2 потомка.
class FullBinaryTree {
public:
    enum Result { OK, EXISTS, NO_PARENT, OCCUPIED, NOT_LEAF };

    FullBinaryTree();
    ~FullBinaryTree();
    FullBinaryTree(const FullBinaryTree&) = delete;
    FullBinaryTree& operator=(const FullBinaryTree&) = delete;

    Result insert(int value);                          // добавление по уровням
    Result insertAt(int parent, char side, int value); // добавление слева/справа
    Result insertPair(int parent, int left, int right);// сразу два потомка

    bool contains(int value) const;
    bool isFull() const;
    std::string preorder() const;                     // корень, левый, правый
    std::string bfs() const;                          // обход по уровням

    // Сохранение/загрузка: порядок по уровням, # — отсутствующий узел.
    std::string serialize() const;
    bool load(const Array& items);

private:
    struct Node {
        int data;
        Node* left;
        Node* right;
    };
    Node* root;
    int count;

    static void clear(Node* node);
    static Node* find(Node* node, int value);
    static bool full(Node* node);
    static void preorderFrom(Node* node, std::string& out);
};

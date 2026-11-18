// Модуль обработки запросов.
//
// Файл данных: одна строка на структуру — "<тип> <имя> <элементы через пробел>".
// Типы: M — массив, F — односвязный список, L — двусвязный список,
//       S — стек, Q — очередь, T — дерево (Full Binary Tree).
// Первая буква команды — тип структуры (MPUSH, FDEL, SPOP ...).
// Исключения: PRINT (тип берётся из файла) и ISMEMBER (дерево).

#include "commands.h"

#include <fstream>
#include <stdexcept>
#include <string>

#include "array.h"
#include "dlist.h"
#include "fbt.h"
#include "flist.h"
#include "queue.h"
#include "stack.h"

namespace {

using std::string;

// ---------- вспомогательные функции ----------

[[noreturn]] void fail(const string& message) {
    throw std::runtime_error(message);
}

// Разбиение строки на слова по пробелам и табуляциям
void split(const string& line, Array& words) {
    string word;
    for (char c : line) {
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            if (!word.empty()) {
                words.push(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push(word);
    }
}

string toUpper(string s) {
    for (char& c : s) {
        if (c >= 'a' && c <= 'z') {
            c = static_cast<char>(c - 'a' + 'A');
        }
    }
    return s;
}

string word(const Array& words, int index) {
    string result;
    words.get(index, result);
    return result;
}

int toInt(const string& s) {
    size_t pos = 0;
    int value = 0;
    try {
        value = std::stoi(s, &pos);
    } catch (...) {
        pos = 0;
    }
    if (pos == 0 || pos != s.size()) {
        fail("'" + s + "' is not an integer");
    }
    return value;
}

// Проверка числа слов в запросе (вместе с командой и именем)
void need(const Array& q, int count, const string& usage) {
    if (q.length() != count) {
        fail("usage: " + usage);
    }
}

string boolText(bool value) {
    return value ? "TRUE" : "FALSE";
}

// ---------- массив ----------

string runArray(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    Array arr;
    for (int i = 0; i < items.length(); i++) {
        arr.push(word(items, i));
    }
    string reply;

    if (cmd == "MCREATE") {
        need(q, 2, "MCREATE name");
        reply = "OK";
        changed = true;
    } else if (cmd == "MPUSH") {
        need(q, 3, "MPUSH name value");
        arr.push(word(q, 2));
        reply = word(q, 2);
        changed = true;
    } else if (cmd == "MINSERT") {
        need(q, 4, "MINSERT name index value");
        if (!arr.insert(toInt(word(q, 2)), word(q, 3))) fail("index out of range");
        reply = word(q, 3);
        changed = true;
    } else if (cmd == "MGET") {
        need(q, 3, "MGET name index");
        if (!arr.get(toInt(word(q, 2)), reply)) fail("index out of range");
    } else if (cmd == "MSET") {
        need(q, 4, "MSET name index value");
        if (!arr.set(toInt(word(q, 2)), word(q, 3))) fail("index out of range");
        reply = word(q, 3);
        changed = true;
    } else if (cmd == "MDEL") {
        need(q, 3, "MDEL name index");
        if (!arr.remove(toInt(word(q, 2)), reply)) fail("index out of range");
        changed = true;
    } else if (cmd == "MLEN") {
        need(q, 2, "MLEN name");
        reply = std::to_string(arr.length());
    } else if (cmd == "PRINT") {
        reply = "[" + arr.toString(", ") + "]";
    } else {
        fail("unknown command " + cmd);
    }
    saved = arr.toString(" ");
    return reply;
}

// ---------- списки ----------
// Односвязный и двусвязный списки имеют одинаковый интерфейс, поэтому команды
// обрабатываются одним шаблоном. arrow — разделитель при выводе.

template <typename List>
string runList(const string& arrow, const string& cmd, const Array& q, const Array& items,
               bool& changed, string& saved) {
    List list;
    for (int i = 0; i < items.length(); i++) {
        list.pushTail(word(items, i));
    }
    const string c = cmd.substr(1); // команда без буквы типа
    string reply;

    if (c == "PUSH" || c == "PUSHTAIL") {
        need(q, 3, cmd + " name value");
        list.pushTail(word(q, 2));
        reply = word(q, 2);
        changed = true;
    } else if (c == "PUSHHEAD") {
        need(q, 3, cmd + " name value");
        list.pushHead(word(q, 2));
        reply = word(q, 2);
        changed = true;
    } else if (c == "PUSHBEFORE") {
        need(q, 4, cmd + " name target value");
        if (!list.pushBefore(word(q, 2), word(q, 3))) fail("element '" + word(q, 2) + "' not found");
        reply = word(q, 3);
        changed = true;
    } else if (c == "PUSHAFTER") {
        need(q, 4, cmd + " name target value");
        if (!list.pushAfter(word(q, 2), word(q, 3))) fail("element '" + word(q, 2) + "' not found");
        reply = word(q, 3);
        changed = true;
    } else if (c == "DELHEAD") {
        need(q, 2, cmd + " name");
        if (!list.delHead(reply)) fail("list is empty");
        changed = true;
    } else if (c == "DELTAIL") {
        need(q, 2, cmd + " name");
        if (!list.delTail(reply)) fail("list is empty");
        changed = true;
    } else if (c == "DELBEFORE") {
        need(q, 3, cmd + " name target");
        if (!list.delBefore(word(q, 2), reply)) fail("nothing before '" + word(q, 2) + "'");
        changed = true;
    } else if (c == "DELAFTER") {
        need(q, 3, cmd + " name target");
        if (!list.delAfter(word(q, 2), reply)) fail("nothing after '" + word(q, 2) + "'");
        changed = true;
    } else if (c == "DEL") {
        need(q, 3, cmd + " name value");
        if (!list.delValue(word(q, 2))) fail("element '" + word(q, 2) + "' not found");
        reply = word(q, 2);
        changed = true;
    } else if (c == "GET") {
        need(q, 3, cmd + " name index");
        if (!list.get(toInt(word(q, 2)), reply)) fail("index out of range");
    } else if (c == "FIND") {
        need(q, 3, cmd + " name value");
        reply = boolText(list.find(word(q, 2)));
    } else if (c == "LEN") {
        need(q, 2, cmd + " name");
        reply = std::to_string(list.length());
    } else if (c == "PRINTREV") {
        need(q, 2, cmd + " name");
        reply = list.length() == 0 ? "(empty)" : list.toStringReverse(arrow);
    } else if (cmd == "PRINT") {
        reply = list.length() == 0 ? "(empty)" : list.toString(arrow);
    } else {
        fail("unknown command " + cmd);
    }
    saved = list.toString(" ");
    return reply;
}

// ---------- стек ----------

string runStack(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    Stack st;
    for (int i = 0; i < items.length(); i++) { // в файле от дна к вершине
        st.push(word(items, i));
    }
    string reply;

    if (cmd == "SPUSH") {
        need(q, 3, "SPUSH name value");
        st.push(word(q, 2));
        reply = word(q, 2);
        changed = true;
    } else if (cmd == "SPOP") {
        need(q, 2, "SPOP name");
        if (!st.pop(reply)) fail("stack is empty");
        changed = true;
    } else if (cmd == "SPEEK") {
        need(q, 2, "SPEEK name");
        if (!st.peek(reply)) fail("stack is empty");
    } else if (cmd == "SLEN") {
        need(q, 2, "SLEN name");
        reply = std::to_string(st.length());
    } else if (cmd == "PRINT") {
        reply = "bottom [" + st.toString(", ") + "] top";
    } else {
        fail("unknown command " + cmd);
    }
    saved = st.toString(" ");
    return reply;
}

// ---------- очередь ----------

string runQueue(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    Queue qu;
    for (int i = 0; i < items.length(); i++) { // в файле от головы к хвосту
        qu.push(word(items, i));
    }
    string reply;

    if (cmd == "QPUSH") {
        need(q, 3, "QPUSH name value");
        qu.push(word(q, 2));
        reply = word(q, 2);
        changed = true;
    } else if (cmd == "QPOP") {
        need(q, 2, "QPOP name");
        if (!qu.pop(reply)) fail("queue is empty");
        changed = true;
    } else if (cmd == "QPEEK") {
        need(q, 2, "QPEEK name");
        if (!qu.peek(reply)) fail("queue is empty");
    } else if (cmd == "QLEN") {
        need(q, 2, "QLEN name");
        reply = std::to_string(qu.length());
    } else if (cmd == "PRINT") {
        reply = "head [" + qu.toString(", ") + "] tail";
    } else {
        fail("unknown command " + cmd);
    }
    saved = qu.toString(" ");
    return reply;
}

// ---------- дерево ----------

void checkTree(FullBinaryTree::Result r, int parent) {
    switch (r) {
        case FullBinaryTree::OK: return;
        case FullBinaryTree::EXISTS: fail("value already in tree (duplicates are not allowed)");
        case FullBinaryTree::NO_PARENT: fail("parent " + std::to_string(parent) + " not found");
        case FullBinaryTree::OCCUPIED: fail("this child of " + std::to_string(parent) + " already exists");
        case FullBinaryTree::NOT_LEAF: fail(std::to_string(parent) + " is not a leaf");
    }
}

string runTree(const string& cmd, const Array& q, const Array& items, bool& changed, string& saved) {
    FullBinaryTree tree;
    if (!tree.load(items)) {
        fail("tree in file is damaged");
    }
    string reply;

    if (cmd == "TINSERT") {
        need(q, 3, "TINSERT name number");
        int v = toInt(word(q, 2));
        checkTree(tree.insert(v), 0);
        reply = word(q, 2);
        changed = true;
    } else if (cmd == "TINSERTAT") {
        need(q, 5, "TINSERTAT name parent L|R number");
        string side = toUpper(word(q, 3));
        if (side != "L" && side != "R") fail("side must be L or R");
        int parent = toInt(word(q, 2));
        int v = toInt(word(q, 4));
        checkTree(tree.insertAt(parent, side[0], v), parent);
        reply = word(q, 4);
        changed = true;
    } else if (cmd == "TPAIR") {
        need(q, 5, "TPAIR name parent left right");
        int parent = toInt(word(q, 2));
        checkTree(tree.insertPair(parent, toInt(word(q, 3)), toInt(word(q, 4))), parent);
        reply = word(q, 3) + " " + word(q, 4);
        changed = true;
    } else if (cmd == "TGET" || cmd == "ISMEMBER") {
        need(q, 3, cmd + " name number");
        reply = boolText(tree.contains(toInt(word(q, 2))));
    } else if (cmd == "TISFULL") {
        need(q, 2, "TISFULL name");
        reply = boolText(tree.isFull());
    } else if (cmd == "TPREORDER") {
        need(q, 2, "TPREORDER name");
        reply = tree.preorder();
    } else if (cmd == "TBFS" || cmd == "PRINT") {
        need(q, 2, cmd + " name");
        reply = tree.bfs();
    } else {
        fail("unknown command " + cmd);
    }
    saved = tree.serialize();
    return reply;
}

// ---------- файл ----------

void readLines(const string& fileName, Array& lines) {
    std::ifstream in(fileName, std::ios::binary);
    if (!in) {
        return; // файла нет — база пустая, создадим при первом изменении
    }
    string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') { // файл, сохранённый в Windows
            line.pop_back();
        }
        Array w;
        split(line, w);
        if (w.length() > 0) {
            lines.push(line);
        }
    }
}

void writeLines(const string& fileName, const Array& lines) {
    // binary — чтобы и на Windows, и на Linux переводы строк были одинаковые "\n"
    std::ofstream out(fileName, std::ios::binary | std::ios::trunc);
    if (!out) {
        fail("cannot write file " + fileName);
    }
    for (int i = 0; i < lines.length(); i++) {
        out << word(lines, i) << '\n';
    }
}

bool createsStructure(const string& cmd) {
    return cmd.find("PUSH") != string::npos || cmd == "MCREATE" || cmd == "MINSERT" ||
           cmd == "TINSERT" || cmd == "TPAIR";
}

} // namespace

std::string ExecuteQuery(const std::string& fileName, const std::string& query) {
    Array q;
    split(query, q);
    if (q.length() < 2) {
        fail("query must be: COMMAND name [args]");
    }
    // команду переводим в верхний регистр и кладём обратно в запрос
    const string cmd = toUpper(word(q, 0));
    const string name = word(q, 1);

    // Ищем структуру в файле
    Array lines;
    readLines(fileName, lines);
    int found = -1;
    char fileType = 0;
    Array items;
    for (int i = 0; i < lines.length() && found < 0; i++) {
        Array w;
        split(word(lines, i), w);
        if (w.length() >= 2 && word(w, 1) == name) {
            found = i;
            fileType = word(w, 0)[0];
            for (int j = 2; j < w.length(); j++) {
                items.push(word(w, j));
            }
        }
    }

    // Определяем тип структуры по команде
    char type;
    if (cmd == "PRINT") {
        if (found < 0) fail("structure '" + name + "' not found");
        type = fileType;
    } else if (cmd == "ISMEMBER") {
        type = 'T';
    } else {
        type = cmd[0];
    }
    if (string("MFLSQT").find(type) == string::npos) {
        fail("unknown command " + cmd);
    }
    if (found >= 0 && fileType != type) {
        fail("'" + name + "' is a structure of type " + string(1, fileType) + ", not " + string(1, type));
    }
    if (found < 0 && !createsStructure(cmd)) {
        fail("structure '" + name + "' not found");
    }
    if (found >= 0 && cmd == "MCREATE") {
        fail("structure '" + name + "' already exists");
    }

    bool changed = false;
    string saved;
    string reply;
    switch (type) {
        case 'M': reply = runArray(cmd, q, items, changed, saved); break;
        case 'F': reply = runList<FList>(" -> ", cmd, q, items, changed, saved); break;
        case 'L': reply = runList<DList>(" <-> ", cmd, q, items, changed, saved); break;
        case 'S': reply = runStack(cmd, q, items, changed, saved); break;
        case 'Q': reply = runQueue(cmd, q, items, changed, saved); break;
        case 'T': reply = runTree(cmd, q, items, changed, saved); break;
    }

    // Файл перезаписываем только если состав элементов изменился
    if (changed) {
        string line = string(1, type) + " " + name;
        if (!saved.empty()) {
            line += " " + saved;
        }
        if (found >= 0) {
            lines.set(found, line);
        } else {
            lines.push(line);
        }
        writeLines(fileName, lines);
    }
    return reply;
}

#include <iostream>
#include <stdexcept>
#include <string>

#include "commands.h"

// Убирает кавычки вокруг запроса. Нужно для Windows cmd.exe: там одинарные кавычки
// не группируют слова, и 'SPUSH s x' приходит тремя аргументами вместе с кавычками.
static std::string stripQuotes(std::string s) {
    while (!s.empty() && (s.front() == '\'' || s.front() == '"')) s.erase(0, 1);
    while (!s.empty() && (s.back() == '\'' || s.back() == '"')) s.pop_back();
    return s;
}

int main(int argc, char* argv[]) {
    std::string fileName;
    std::string query;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            fileName = argv[++i];
        } else if (arg == "--query") {
            // собираем все слова до следующего ключа --...
            while (i + 1 < argc && std::string(argv[i + 1]).rfind("--", 0) != 0) {
                if (!query.empty()) query += ' ';
                query += argv[++i];
            }
        }
    }
    query = stripQuotes(query);

    if (fileName.empty() || query.empty()) {
        std::cout << "usage: dbms --file <file> --query \"<COMMAND name [args]>\"" << std::endl;
        return 1;
    }

    try {
        std::string reply = ExecuteQuery(fileName, query);
        std::cout << "-> " << reply << std::endl;
    } catch (const std::exception& e) {
        std::cout << "-> ERROR: " << e.what() << std::endl;
        return 2;
    }
    return 0;
}

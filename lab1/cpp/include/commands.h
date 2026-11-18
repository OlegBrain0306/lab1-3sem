#pragma once

#include <string>

// Выполняет запрос query над файлом данных fileName и возвращает ответ.
// При ошибке бросает std::runtime_error с текстом ошибки, файл при этом не меняется.
std::string ExecuteQuery(const std::string& fileName, const std::string& query);

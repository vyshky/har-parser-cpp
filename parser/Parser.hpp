//
// Created by Revhome on 28.09.2025.
//

#ifndef HAR_PARSER_APP_PARCER_H
#define HAR_PARSER_APP_PARCER_H

#include <string>
#include <vector>
#include "../include/Entry.hpp"
#include "../include/Occurrence.hpp"
#include "../include/Variable.hpp"
#include "../include/Indexes.hpp"
#include "../lib/simdjson/simdjson.h"

namespace simdjson::dom {
    class element;
}

class Parser {
public:
    Parser() = default;

    // 🔹 Загружает HAR-файл в память (DOM)
    bool loadHarFile(const std::string &filename);

    // 🔹 Возвращает все Entry после разбора
    const std::vector<Entry> &getEntries() const { return entries; }

    // 🔹 Построение inverted index по ключам и значениям
    void buildIndexes();

private:
    std::vector<Entry> entries; // Все Entry из HAR
    InvertedIndex invertedIndex; // value -> Occurrences
    KeyIndex keyIndex; // key -> (value, Occurrence)
    VariableMap variables; // имя переменной -> Variable

    // 🔹 Разбор одного Entry
    void parseEntry(const simdjson::dom::element &elem, uint64_t id);

    // 🔹 Извлечение key-value из JSON строки
    void extractKeyValues(const std::string &jsonStr, uint64_t entry_id, const std::string &location);

    // 🔹 Извлечение параметров из URL (query string)
    void extractQueryParams(const std::string &url, uint64_t entry_id);

    // 🔹 Извлечение параметров из request body (для form data / JSON)
    void extractRequestParams(const std::string &body, uint64_t entry_id);
};

#endif //HAR_PARSER_APP_PARCER_H

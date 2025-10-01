//
// Created by Revhome on 28.09.2025.
//

#include "Parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>

bool Parser::loadHarFile(const std::string &filename) {
    try {
        simdjson::dom::parser parser;
        const simdjson::dom::element doc = parser.load(filename);

        auto entriesArray = doc["log"]["entries"];
        if (entriesArray.error()) {
            std::cerr << "No entries found in HAR\n";
            return false;
        }

        uint64_t id = 0;
        for (auto elem : entriesArray.value()) {
            parseEntry(elem, id++);
        }

    } catch (const simdjson::simdjson_error &e) {
        std::cerr << "Error parsing HAR: " << e.what() << "\n";
        return false;
    }
    return true;
}

void Parser::parseEntry(const simdjson::dom::element &elem, const uint64_t id) {
    Entry entry;
    entry.id = id;

    // Время начала запроса
    entry.startedDateTime = std::string(elem["startedDateTime"].get_c_str().value());

    // URL и метод
    entry.url = std::string(elem["request"]["url"].get_c_str().value());
    entry.method = std::string(elem["request"]["method"].get_c_str().value());

    // HTTP статус
    entry.status = elem["response"]["status"].get_uint64().value();

    // Request body
    if (const auto reqBodyElem = elem["request"]["postData"]["text"]; !reqBodyElem.error()) {
        entry.request_body = std::string(reqBodyElem.get_c_str().value());
    }

    // Response body
    if (const auto resBodyElem = elem["response"]["content"]["text"]; !resBodyElem.error()) {
        entry.response_text = std::string(resBodyElem.get_c_str().value());
    }

    // Заголовки request и response (для простоты храним как строку)
    if (auto reqHeadersElem = elem["request"]["headers"]; !reqHeadersElem.error()) {
        for (auto h : reqHeadersElem.value()) {
            entry.request_body += h["name"].get_c_str().value();
            entry.request_body += ":";
            entry.request_body += h["value"].get_c_str().value();
            entry.request_body += "\n";
        }
    }

    if (auto resHeadersElem = elem["response"]["headers"]; !resHeadersElem.error()) {
        for (auto h : resHeadersElem.value()) {
            entry.response_text += h["name"].get_c_str().value();
            entry.response_text += ":";
            entry.response_text += h["value"].get_c_str().value();
            entry.response_text += "\n";
        }
    }

    entries.push_back(std::move(entry));

    // Извлекаем query параметры
    extractQueryParams(entries.back().url, id);

    // Извлекаем параметры из request body
    extractRequestParams(entries.back().request_body, id);

    // Извлекаем key-values из response (JSON)
    extractKeyValues(entries.back().response_text, id, "response");
}

// ---------------------------
// Извлечение query-параметров из URL
void Parser::extractQueryParams(const std::string &url, const uint64_t entry_id) {
    const auto pos = url.find('?');
    if (pos == std::string::npos) return;
    const std::string query = url.substr(pos + 1);

    const std::regex re("([^&=]+)=([^&]*)");
    std::smatch match;
    std::string s = query;
    while (std::regex_search(s, match, re)) {
        Occurrence occ;
        occ.entry_id = entry_id;
        occ.location = "query";
        occ.json_path = match[1];
        occ.timestamp = entry_id;
        invertedIndex[match[2]].push_back(occ);
        keyIndex[match[1]].emplace_back(match[2], occ);
        s = match.suffix().str();
    }
}

// ---------------------------
// Извлечение параметров из request body (JSON или form data)
void Parser::extractRequestParams(const std::string &body, uint64_t entry_id) {
    // Если JSON
    simdjson::dom::parser parser;
    simdjson::dom::element elem;
    if (parser.parse(body).get(elem) == simdjson::SUCCESS) {
        for (auto kv : elem.get_object()) {
            std::string key = std::string(kv.key);
            std::string value = kv.value.is_string() ? std::string(kv.value.get_c_str().value())
                                                     : std::to_string(kv.value.get_uint64().value());
            Occurrence occ;
            occ.entry_id = entry_id;
            occ.location = "request_body";
            occ.json_path = key;
            occ.timestamp = entry_id;
            invertedIndex[value].push_back(occ);
            keyIndex[key].emplace_back(value, occ);
        }
    } else {
        // Если form data: key=value&key2=value2
        std::regex re("([^&=]+)=([^&]*)");
        std::smatch match;
        std::string s = body;
        while (std::regex_search(s, match, re)) {
            Occurrence occ;
            occ.entry_id = entry_id;
            occ.location = "request_body";
            occ.json_path = match[1];
            occ.timestamp = entry_id;
            invertedIndex[match[2]].push_back(occ);
            keyIndex[match[1]].emplace_back(match[2], occ);
            s = match.suffix().str();
        }
    }
}

// ---------------------------
void Parser::extractKeyValues(const std::string &jsonStr, uint64_t entry_id, const std::string &location) {
    simdjson::dom::parser parser;
    simdjson::dom::element elem;
    if (parser.parse(jsonStr).get(elem)) return;

    for (auto kv : elem.get_object()) {
        auto key = std::string(kv.key);
        std::string value = kv.value.is_string() ? std::string(kv.value.get_c_str().value())
                                                 : std::to_string(kv.value.get_uint64().value());

        Occurrence occ;
        occ.entry_id = entry_id;
        occ.location = location;
        occ.json_path = key;
        occ.timestamp = entry_id;
        invertedIndex[value].push_back(occ);
        keyIndex[key].emplace_back(value, occ);
    }
}

void Parser::buildIndexes() {
    // Пока invertedIndex и keyIndex строятся сразу при parse
    // Здесь можно добавить логику promote переменных и проверки повторного использования
}

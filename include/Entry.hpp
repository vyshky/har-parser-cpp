//
// Created by Revhome on 28.09.2025.
//

#ifndef HAR_PARSER_APP_ENTRY_H
#define HAR_PARSER_APP_ENTRY_H

#include <string>
#include <cstdint>  // для uint64_t

// 📦 Структура, представляющая один HTTP-запрос/ответ из HAR
struct Entry {
    uint64_t id;                  // Уникальный ID записи (для связи с индексами)
    std::string startedDateTime;  // Время начала запроса, для сортировки по времени
    std::string method;           // HTTP метод (GET, POST, PUT…)
    std::string url;              // URL запроса
    unsigned long long status;                   // HTTP статус ответа (200, 404…)
    std::string request_body;     // Тело запроса, если есть
    std::string response_text;    // Тело ответа (JSON, текст, HTML)
};
#endif //HAR_PARSER_APP_ENTRY_H

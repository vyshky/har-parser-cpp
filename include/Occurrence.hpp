//
// Created by Revhome on 28.09.2025.
//

#ifndef HAR_PARSER_APP_OCCURRENCE_HPP
#define HAR_PARSER_APP_OCCURRENCE_HPP

#include <string>
#include <cstdint>

// 🕵️‍♂️ Структура для хранения информации о том, где и как встречается значение
struct Occurrence {
    uint64_t entry_id;        // ID Entry, в котором найдено значение
    std::string location;     // Где найдено: response, request.url, request.body, header
    std::string json_path;    // Путь до ключа в JSON (например "user.clientId")
    uint64_t timestamp;       // Для сортировки по времени
};

#endif //HAR_PARSER_APP_OCCURRENCE_HPP

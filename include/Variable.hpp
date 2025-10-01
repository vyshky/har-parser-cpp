//
// Created by Revhome on 28.09.2025.
//

#ifndef HAR_PARSER_APP_VARIABLE_HPP
#define HAR_PARSER_APP_VARIABLE_HPP

#include <string>
#include "Occurrence.hpp"

// 🏷 Структура переменной для генерации шаблонов ${clientId}
struct Variable {
    std::string name;      // Имя переменной, например "clientId"
    std::string value;     // Значение переменной, например "1500"
    Occurrence source;     // Откуда взято значение (Entry + JSON путь)
};

#endif //HAR_PARSER_APP_VARIABLE_HPP

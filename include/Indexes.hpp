//
// Created by Revhome on 28.09.2025.
//

#ifndef HAR_PARSER_APP_INDEXES_HPP
#define HAR_PARSER_APP_INDEXES_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "Occurrence.hpp"
#include "Variable.hpp"

// 🔑 Inverted index: value -> список вхождений
using InvertedIndex = std::unordered_map<std::string, std::vector<Occurrence>>;

// key -> список пар (значение, occurrence)
using KeyIndex = std::unordered_map<std::string, std::vector<std::pair<std::string, Occurrence>>>;

// Имя переменной -> структура Variable
using VariableMap = std::unordered_map<std::string, Variable>;

#endif //HAR_PARSER_APP_INDEXES_HPP

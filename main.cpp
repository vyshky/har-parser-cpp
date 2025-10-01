#include "parser/Parser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.har>\n";
        return 1;
    }

    std::string harFile = argv[1];
    Parser parser;

    if (!parser.loadHarFile(harFile)) {
        std::cerr << "Failed to load HAR file: " << harFile << "\n";
        return 1;
    }

    //parser.buildIndexes(); // если нужны индексы после парса

    // std::cout << "Parsed " << parser.entries.size() << " HAR entries\n";
    //
    // // Пример: вывод первых 5 URL
    // for (size_t i = 0; i < parser.entries.size() && i < 5; ++i) {
    //     std::cout << i << ": " << parser.entries[i].url << " ["
    //               << parser.entries[i].method << "] Status: "
    //               << parser.entries[i].status << "\n";
    // }

    return 0;
}

#include <iostream>
#include "FileScanner.h"

int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Usage: ./indexer <folder_path> <word>\n";
        return 1;
    }

    std::string folder = argv[1];
    std::string word = argv[2];

    FileScanner scanner;
    auto results = scanner.search(folder, word);

    std::cout << "Found in:\n";
    for (const auto& file : results) {
        std::cout << "- " << file << "\n";
    }

    return 0;

}
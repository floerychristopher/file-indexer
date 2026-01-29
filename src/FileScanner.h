#pragma once // "Diese Datei nur einmal einbinden"
#include <string>
#include <vector>

class FileScanner {
    public:
        std::vector<std::string> search(const std::string& rootPath, const std::string& word);

    private:
        void scan(const std::string& path, const std::string& word, std::vector<std::string>& results);
}
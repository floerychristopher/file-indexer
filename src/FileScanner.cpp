#include "FileScanner.h"
#include <filesystem> //Für Ordner/Datein
#include <fstream> //Datein lesen
#include <iostream> //Ausgabe
#include <thread>
#include <atomic>

namespace fs = std::filesystem;

void scanDirectory(const std::string& path, ThreadPool& pool) {

    static std::mutex coutMutex;

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Scanning folder: " << path << " | Thread: " << std::this_thread::get_id() << "\n";
    }
    
    for (const auto& entry : fs::directory_iterator(path)) {

        if (entry.is_directory()) {
            std::string subPath = entry.path().string();

            // Neue Aufgabe in den Pool
            pool.enqueue([subPath, &pool]() {
                scanDirectory(subPath, pool);
            });
        }

        else if (entry.is_regular_file()) {
            {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Found file: " << entry.path() << " | Thread: " << std::this_thread::get_id() << "\n";
            }
        }
    }
}
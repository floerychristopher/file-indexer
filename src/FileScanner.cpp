#include "FileScanner.h"
#include <filesystem> //Für Ordner/Datein
#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>

namespace fs = std::filesystem;

void scanDirectory(const fs::path& path, ThreadPool& pool) {
    static std::mutex coutMutex;

    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "Scanning folder: " << path << " | Thread: " << std::this_thread::get_id() << "\n";
    }
    
    std::vector<fs::path> fileBatch; // Vector for batch of files 
    const size_t batchSize = 1;      // Batch size

    for (const auto& entry : fs::directory_iterator(path)) {

        if (entry.is_directory()) {
            fs::path subPath = entry.path();

            // Directory as task enqueued
            pool.enqueue([subPath, &pool]() {
                std::cout << "Ordner " << subPath << " wurde als Task in Queue aufgenommen.\n";
                scanDirectory(subPath, pool);
            });
        }

        else if (entry.is_regular_file()) {
            fileBatch.push_back(entry.path());

            if (fileBatch.size() >= batchSize) {
                auto batchCopy = fileBatch;
                pool.enqueue([batchCopy]() {
                    for (const auto& file : batchCopy) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        std::cout << "Found file: " << file << " | Thread: " << std::this_thread::get_id() << "\n";
                    }
                });
                fileBatch.clear();
            }
        }
    }

    if (!fileBatch.empty()) {
        auto batchCopy = fileBatch;
        pool.enqueue([batchCopy]() {
            for (const auto& file : batchCopy) {
                std::cout << "Found file: " << file << " | Thread: " << std::this_thread::get_id() << "\n";
            }
        });
    }
}
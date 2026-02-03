#include <iostream>
#include <thread>
#include "ThreadPool.h"
#include "FileScanner.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./indexer <path>\n";
        return 1;
    }

    std::string startPath = argv[1];

    size_t numThreads = std::thread::hardware_concurrency();
    std::cout << "Starting ThreadPool with " << numThreads << " threads\n";

    ThreadPool pool(numThreads);

    // Erste Aufgabe
    pool.enqueue([startPath, &pool]() {
        scanDirectory(startPath, pool);
    }); 

    return 0;

}
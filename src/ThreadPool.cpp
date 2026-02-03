#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        // Wir erstellen jetzt numThreads Threads, rufen dafür emplace_back auf, welcher eine Lambda Funktion aufruft, welche auf alle Member der ThreadPool-Klasse zugreifen soll ([this])
        workers.emplace_back([this]() {
            // Jeder Threat läuft endlos in der Schleife bis er explizit beendet wird
            while (true) {
                // Momentane Aufgabe deklariert
                std::function<void()> task;
                // Block-Scope durch {}
                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    // Thread schläft bis stop == true ODER tasks nicht leer
                    condition.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });

                    // Thread beendet sich wenn stop UND tasks.empty() true sind
                    if (stop && tasks.empty()) {
                        return;
                    }

                    // Nimmt nächste Aufgabe aus Queue
                    task = std::move(tasks.front());
                    tasks.pop();
                }

                task(); //Task ausführen
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }

    condition.notify_all();

    for (std::thread &worker : workers) {
        worker.join();
    }
}
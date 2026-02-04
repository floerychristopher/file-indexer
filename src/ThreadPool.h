#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

class ThreadPool {
    public:
        ThreadPool(size_t numThreads);
        ~ThreadPool();

        void enqueue(std::function<void()> task);

    private:
        std::vector<std::thread> workers;           // Workers
        std::queue<std::function<void()>> tasks;    // Task queue

        std::mutex queueMutex;                      // Protect queue
        std::condition_variable condition;          // Wakes up waiting threads
        bool stop;
};

#endif
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
    public:
        ThreadPool(size_t numThreads);
        ~ThreadPool();

        void enqueue(std::function<void()> task);

    private:
        std::vector<std::thread> workers;           // Workers
        std::queue<std::function<void()>> tasks;    // Task Warteschlange

        std::mutex queueMutex;                      // schützt die Queue
        std::condition_variable condition;          // weckt schlafende Threads
        bool stop;
};

#endif
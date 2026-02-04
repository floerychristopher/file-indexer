#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {   
    for (size_t i = 0; i < numThreads; ++i) {

        // Creates thread which executes lambda function
        workers.emplace_back([this]() {
            //std::cout << "Thread " << std::this_thread::get_id() << " has been created.\n";
            // Thread is running until explicitly turned off
            while (true) {

                // Declare current task (empty function)
                std::function<void()> task;

                // Block scope for lock
                {
                    // Lock declaration + already locks the mutex (until block scope is over)
                    std::unique_lock<std::mutex> lock(queueMutex);

                    // Thread is waiting
                    // Wakes up if stop==true (finish tasks and turn off) or !tasks.empty() (finish tasks and go back to waiting) 
                    // Unlock → sleep → wake → lock (wait for other threads to unlock) → continue code
                    //std::cout << "Thread " << std::this_thread::get_id() << " is now asleep.\n";
                    condition.wait(lock, [this]() {
                        return stop || !tasks.empty();
                    });
                    //std::cout << "Thread " << std::this_thread::get_id() << " woke up.\n";

                    // Thread turns off if stop AND tasks.empty() are true
                    if (stop && tasks.empty()) {
                        return;
                    }

                    // Takes next task from queue
                    task = std::move(tasks.front());
                    tasks.pop();
                }

                task(); // Execute task
            }
        });
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        // Push passed function task onto queue "tasks"
        tasks.push(std::move(task));
        std::cout << "tasks.size() == " << tasks.size() << "\n"; 
    }
    // Wake one random thread (OS decides)
    condition.notify_one();
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stop = true;
    }

    // Wake all sleeping threads
    condition.notify_all();

    for (std::thread &worker : workers) {
        worker.join();
    }
}
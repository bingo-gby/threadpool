#ifndef THREADPOOL_H
#define THREADPOOL_H
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<queue>
#include<functional>
#include<vector>

class ThreadPool {
public:
    ThreadPool(size_t threadCount = 10);
    static std::shared_ptr<ThreadPool> getInstance();
    ~ThreadPool();
    void addTask(std::function<void()>&& task) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.push(task);
        }
        condition.notify_all();
    }
    void waitAllTasks() {
        while (true) {
            if (tasks.empty()) {
                break;
            }
        }
    }
private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<std::function<void()>> tasks;
    bool isClosed = false;
    std::vector<std::thread> threads;
};
#endif // THREADPOOL_H


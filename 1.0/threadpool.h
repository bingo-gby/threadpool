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
    // 写个模板
    template<typename T,typename... Args>
    void addTask(T&& fun, Args&&... args ) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            auto task = std::bind(std::forward<T>(fun),std::forward<Args>(args)...);  // ... 放在最后
            tasks.emplace(task);
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


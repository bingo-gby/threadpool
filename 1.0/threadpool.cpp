#include "threadpool.h"

ThreadPool::ThreadPool(size_t threadCount) {
    for (size_t i = 0; i < threadCount; i++) {
        threads.emplace_back(
            std::thread([this] {
                std::unique_lock<std::mutex> lock(mutex);
                while (true) {
                    if(isClosed) {
                        break;
                    }
                    if(!tasks.empty()) {
                        auto task = tasks.front();
                        tasks.pop();
                        lock.unlock();
                        task();
                        lock.lock();  // 重新加锁,便于继续取任务
                    } else {
                        condition.wait(lock);
                    }
                }
            })
        );
    }
}

// 懒汉模式，用时创建
std::shared_ptr<ThreadPool> ThreadPool::getInstance() {
    static auto instance = std::make_shared<ThreadPool>();
    return instance;
}

// 通知所有的线程消费
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(mutex);
        isClosed = true;
    }
    condition.notify_all();
    // 等待所有线程结束
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
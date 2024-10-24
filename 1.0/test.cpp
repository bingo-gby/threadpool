#include "threadpool.cpp"
#include <iostream>
#include <vector>
#include <chrono>

void sleep(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main(){
    // 单例就无法指定线程数
    auto pool = ThreadPool::getInstance();
    std::vector<std::function<void()>> tasks;
    for (int i = 0; i < 10000; i++) {
        tasks.push_back([i] {
            // std::cout << "task " << i << " start" << std::endl;
            // std::cout << "task " << i << " end" << std::endl;
            sleep(0.01);
        });
        std::cout << "add task " << i << std::endl;
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (auto& task : tasks) {
        pool->addTask(std::move(task));
    }
    pool->waitAllTasks();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "addTask time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    
    return 0;
}
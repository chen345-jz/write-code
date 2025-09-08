#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>

// 测试函数1：简单的加法函数
int add(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return a + b;
}

// 测试函数2：字符串处理函数
std::string process_string(const std::string& str, int repeat) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::string result;
    for (int i = 0; i < repeat; ++i) {
        result += str + " ";
    }
    return result;
}

// 测试函数3：无返回值的函数，用于计数
void increment_counter(std::atomic<int>& counter, int delay_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    counter++;
    std::cout << "Counter incremented to: " << counter.load() << std::endl;
}

int main() {
    std::cout << "=== 线程池测试开始 ===" << std::endl;
    
    // 获取线程池实例
    ThreadPool& pool = ThreadPool::instance();
    std::cout << "线程池创建成功，线程数量: " << pool.ThreadCount() << std::endl;
    
    // 测试1：基本功能测试 - 提交有返回值的任务
    std::cout << "\n--- 测试1: 有返回值的任务 ---" << std::endl;
    std::vector<std::future<int>> futures;
    
    for (int i = 0; i < 10; ++i) {
        auto future = pool.commit(add, i, i + 1);
        futures.push_back(std::move(future));
    }
    
    // 获取结果
    for (int i = 0; i < futures.size(); ++i) {
        int result = futures[i].get();
        std::cout << "任务 " << i << " 结果: " << result << std::endl;
    }
    
    // 测试2：字符串处理任务
    std::cout << "\n--- 测试2: 字符串处理任务 ---" << std::endl;
    std::vector<std::future<std::string>> string_futures;
    
    for (int i = 0; i < 5; ++i) {
        auto future = pool.commit(process_string, "Hello", i + 1);
        string_futures.push_back(std::move(future));
    }
    
    for (int i = 0; i < string_futures.size(); ++i) {
        std::string result = string_futures[i].get();
        std::cout << "字符串任务 " << i << " 结果: " << result << std::endl;
    }
    
    // 测试3：无返回值的任务（Lambda表达式）
    std::cout << "\n--- 测试3: Lambda表达式任务 ---" << std::endl;
    std::atomic<int> counter{0};
    std::vector<std::future<void>> void_futures;
    
    for (int i = 0; i < 8; ++i) {
        auto future = pool.commit([&counter, i]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            counter++;
            std::cout << "Lambda任务 " << i << " 完成，计数器: " << counter.load() << std::endl;
        });
        void_futures.push_back(std::move(future));
    }
    
    // 等待所有Lambda任务完成
    for (auto& future : void_futures) {
        future.get();
    }
    
    // 测试4：使用普通函数指针的任务
    std::cout << "\n--- 测试4: 函数指针任务 ---" << std::endl;
    std::atomic<int> counter2{0};
    std::vector<std::future<void>> counter_futures;
    
    for (int i = 0; i < 6; ++i) {
        auto future = pool.commit(increment_counter, std::ref(counter2), 80);
        counter_futures.push_back(std::move(future));
    }
    
    // 等待所有计数任务完成
    for (auto& future : counter_futures) {
        future.get();
    }
    
    std::cout << "最终计数器值: " << counter2.load() << std::endl;
    
    // 测试5：性能测试
    std::cout << "\n--- 测试5: 性能测试 ---" << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<int>> perf_futures;
    const int task_count = 100;
    
    for (int i = 0; i < task_count; ++i) {
        auto future = pool.commit([i]() -> int {
            // 模拟一些计算工作
            int sum = 0;
            for (int j = 0; j < 1000; ++j) {
                sum += j;
            }
            return sum + i;
        });
        perf_futures.push_back(std::move(future));
    }
    
    // 等待所有任务完成
    int total_sum = 0;
    for (auto& future : perf_futures) {
        total_sum += future.get();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "完成 " << task_count << " 个任务，总和: " << total_sum 
              << "，耗时: " << duration.count() << " 毫秒" << std::endl;
    
    // 测试6：异常处理
    std::cout << "\n--- 测试6: 异常处理 ---" << std::endl;
    try {
        auto future = pool.commit([]() -> int {
            throw std::runtime_error("测试异常");
            return 42;
        });
        
        int result = future.get(); // 这里会抛出异常
        std::cout << "不应该到达这里" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "成功捕获异常: " << e.what() << std::endl;
    }
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    std::cout << "当前线程池线程数量: " << pool.ThreadCount() << std::endl;
    
    // 等待一段时间确保所有任务都完成
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    return 0;
}

#include "ThreadPool.h"

ThreadPool::ThreadPool(int threadnums) : isstop(false)
{
    for (size_t i = 0; i < threadnums; i++)
    {
        workers.emplace_back([this]
                             { this->worker(); });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(mtx);
        isstop = true;
    }

    cv.notify_all(); // 通知所有阻塞中的线程

    // 等待所有线程结束
    for (std::thread &onethread : workers)
    {
        if (onethread.joinable())
        {
            onethread.join();
        }
    }
}

void ThreadPool::worker()
{
    while (true)
    {
        std::function<void()> task;

        // 从队列中取得一个任务
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]
                    { return this->isstop || !this->myque.empty(); });
            if (isstop && myque.empty())
                return;
            task = std::move(this->myque.front());
            this->myque.pop();
        }

        // 执行任务
        task();
    }
}

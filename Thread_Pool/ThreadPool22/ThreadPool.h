#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <functional>
#include <queue>

// 线程池
class ThreadPool
{
public:
    ThreadPool(int threadnums);
    template <typename F, typename... Arg>
    // future函数用于获取异步任务的返回值
    // F是可调用对象类型(函数、lambda等), Arg是参数包类型
    // std::result_of用于推导F调用参数Arg后的返回值类型
    // 返回future对象,可以通过future.get()获取任务的返回值
    std::future<typename std::result_of<F(Arg...)>::type> enques(F &&f, Arg &&...arg);
    ~ThreadPool();

private:
    void worker();                           // 工作线程
    bool isstop;                             // 是否停止
    std::condition_variable cv;              // 条件变量
    std::mutex mtx;                          // 互斥锁
    std::vector<std::thread> workers;        // 工作线程
    std::queue<std::function<void()>> myque; // 任务队列
};

template <typename F, typename... Arg>
inline std::future<typename std::result_of<F(Arg...)>::type> ThreadPool::enques(F &&f, Arg &&...arg)
{
    // 获得f执行后的类型
    using functype = typename std::result_of<F(Arg...)>::type;

    // 获得一个智能指针,指向一个被包装为functype的task
    auto task = std::make_shared<std::packaged_task<functype()>>(
        std::bind(std::forward<F>(f), std::forward<Arg>(arg)...));

    // 获得future
    std::future<functype> rsfuture = task->get_future();

    // 将任务添加到队列
    {
        std::lock_guard<std::mutex> lockguard(this->mtx);
        if (isstop)
        {
            throw std::runtime_error("线程池已停止");
        }

        myque.emplace([task]()
                      { (*task)(); });
    }

    cv.notify_one(); // 通知一个线程

    return rsfuture;
}

#include<iostream>
#include<mutex>
#include<condition_variable>
#include<atomic>
#include<queue>
#include<thread>
#include<future>
class ThreadPool
{
public:
    ThreadPool(const ThreadPool&)=delete;
    ThreadPool& operator =(const ThreadPool&)=delete;
    static ThreadPool& instance()
    {
        static ThreadPool ins;
        return ins;
    }

    ~ThreadPool(){
        stop();
    }

    using Task = std::packaged_task<void()>; //用于包装一个任务，使其可以和std::future配合使用，获取任务结果
    template <class F,class... Arg>
    auto commit(F&& f,Arg&&... args)->std::future<decltype(f(args...))>{
        using RetType=decltype(f(args...));
        if(stop_.load())
        return std::future<RetType>{}; //返回一个future对象

        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Arg>(args)...));

        std::future<RetType> ret = task->get_future();

        {
            std::lock_guard<std::mutex> cv_mt(cv_mt_);
            tasks_.emplace([task] { (*task)(); });
        }

        cv_lock_.notify_one();
        return ret;
    }

    int ThreadCount()
    {
        return thread_num_;
    }
private:
    std::mutex               cv_mt_;           // 用于任务队列的互斥锁
    std::condition_variable  cv_lock_;         // 条件变量，用于线程同步
    std::atomic_bool         stop_;            // 线程池停止标志
    std::atomic_int          thread_num_;      // 线程池中的线程数量
    std::queue<Task>         tasks_;           // 任务队列
    std::vector<std::thread> pool_;            // 线程池容器

    ThreadPool(unsigned int num=5):stop_(false)
    {
        if(num<1)
        thread_num_=1;
        else
        thread_num_=num;
        start();
    }

    void start()
    {
        for(int i=0;i<thread_num_;++i)
        {
            pool_.emplace_back([this](){
                while(!this->stop_.load())
                {
                    Task task;
                    {
                        std::unique_lock<std::mutex> cv_mt(cv_mt_);
                        this->cv_lock_.wait(cv_mt,[this]{return this->stop_.load()||!this->tasks_.empty();});
                        if(this->stop_.load() && this->tasks_.empty()) {
                            break;
                        }
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    this->thread_num_--;
                    task();
                    this->thread_num_++;
                }
            });
        }
    }
    
    void stop() {
        stop_.store(true);
        cv_lock_.notify_all();
        for (auto& td : pool_) {
            if (td.joinable()) {
                std::cout << "join thread " << td.get_id() << std::endl;
                td.join();
            }
        }
    }
};
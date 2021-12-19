#pragma once
#include <thread>
#include <queue>
#include <functional>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <memory>

/* 最大线程数量 */
#define MAXTHREADNUMBERS (16)

namespace std
{
    class ThreadPool
    {
    public:
        ThreadPool(size_t maxThreadNums);
        ThreadPool() : ThreadPool(10){};
        ~ThreadPool();
        template <typename FunctionType, typename... ArgsType>
        auto addTask(FunctionType &&func, ArgsType &&...args) -> future<decltype(func(args...))>;

    private:
        queue<function<void()>> m_taskQueue;
        vector<thread> m_threadPool;
        condition_variable m_conditon;
        mutex m_mutex;
        atomic<bool> is_shutdown;
    };
    template <typename FunctionType, typename... ArgsType>
    auto
    ThreadPool::addTask(FunctionType &&func, ArgsType &&...args) -> future<decltype(func(args...))>
    {
        using retType = decltype(func(args...));
        auto task = make_shared<packaged_task<retType()>>(bind(forward<FunctionType>(func),
                                                               forward<ArgsType>(args)...));
        future<retType> retVal = task->get_future();
        /* add to queue */
        {
            unique_lock<mutex> lock(m_mutex);
            m_taskQueue.emplace(
                [task]()
                {
                    (*task)();
                });
        }
        m_conditon.notify_one();
        return retVal;
    }
}
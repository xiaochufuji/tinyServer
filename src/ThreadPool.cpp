#include "../include/ThreadPool.h"

namespace std
{
    ThreadPool::ThreadPool(size_t maxThreadNums)
    {
        is_shutdown = false;
        maxThreadNums = maxThreadNums >= MAXTHREADNUMBERS ? MAXTHREADNUMBERS : maxThreadNums;
        for (size_t i = 0; i < maxThreadNums; ++i)
        {
            m_threadPool.emplace_back(
                [this]
                {
                    while (!this->is_shutdown)
                    {
                        function<void()> task;
                        /* fecth a task */
                        {
                            unique_lock<mutex> lock(this->m_mutex);
                            this->m_conditon.wait(lock,
                                                  [this]() -> bool
                                                  {
                                                      return this->is_shutdown || !this->m_taskQueue.empty();
                                                  });
                            /* 如果是shutdown信号 */
                            if (this->is_shutdown)
                            {
                                return;
                            }
                            /* 不是shutdown信号并且任务队列不为空则取出任务并执行 */
                            task = this->m_taskQueue.front();
                            this->m_taskQueue.pop();
                        }
                        task();
                    }
                });
        }
    }
    ThreadPool::~ThreadPool()
    {
        is_shutdown = true;
        /* 唤醒所有线程 */
        m_conditon.notify_all();
        for (auto &t : m_threadPool)
        {
            /* 线程等待 */
            if (t.joinable())
            {
                t.join();
            }
        }
    }
}
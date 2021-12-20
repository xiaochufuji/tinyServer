#include <functional>
#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include "Log.h"
#include "ThreadPool.h"
using namespace std;

void runtimeCalculate(void (*callback)())
{
    clock_t startTime, endTime;
    startTime = clock(); //计时开始
    callback();
    endTime = clock(); //计时结束
    cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

/* for lamada */
void runtimeCalculate(function<void()> callback)
{
    clock_t startTime, endTime;
    startTime = clock(); //计时开始
    callback();
    endTime = clock(); //计时结束
    cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

template <typename FunctionType, typename... Args>
void runtimeCalculate(FunctionType &&callback, Args... args)
{
    clock_t startTime, endTime;
    startTime = clock(); //计时开始
    callback(args...);
    endTime = clock(); //计时结束
    cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
}

namespace xcfj
{
    using namespace std;
    void test_for_log_and_mutiThread()
    {
        LOGSETLEVEL(LOG_LEVEL_DEBUG);
        LOGSETTARGET(LOG_TARGET_FILE);
        auto a = []()
        {
            int i = 1000;
            while (i--)
            {
                this_thread::sleep_for(chrono::microseconds(rand() % 500 + 500));
                LOG(LOG_LEVEL_INFO, "log", "dosomething", 100, "%s [%u]",
                    "我的那些潦草诗", rand() % 10);
            }
        };
        auto b = thread(a);
        auto c = thread(a);
        auto d = thread(a);
        auto e = thread(a);
        auto f = thread(a);
        auto g = thread(a);
        auto h = thread(a);
        auto i = thread(a);
        auto j = thread(a);
        auto k = thread(a);
        b.join();
        c.join();
        d.join();
        e.join();
        f.join();
        g.join();
        h.join();
        i.join();
        j.join();
        k.join();
    }
    void test_for_log_and_threadPool()
    {
        LOGSETLEVEL(LOG_LEVEL_DEBUG);
        LOGSETTARGET(LOG_TARGET_FILE);
        auto a = []()
        {
            int i = 1000;
            while (i--)
            {
                this_thread::sleep_for(chrono::microseconds(rand() % 500 + 500));
                LOG(LOG_LEVEL_INFO, "log", "dosomething", 100, "%s [%u]",
                    "我的那些潦草诗", rand() % 65535 + 65535);
            }
        };
        ThreadPool pool(10);
        for (size_t i = 0; i < 10; ++i)
        {
            pool.addTask(a);
        }
    }
}
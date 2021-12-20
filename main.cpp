// #include "./include/ThreadPool.h"
// using namespace std;
// #include <iostream>

// double _add(int a, double b)
// {
//     cout << "thread id :" << this_thread::get_id() << endl;
//     return a * b + b * 1.1;
// }

// int main()
// {
//     ThreadPool pool(10);
//     vector<future<double>> resultList;
//     for (int i = 0; i < 10; ++i)
//     {
//         // resultList.emplace_back(
//         //     pool.addTask(
//         //         [i]
//         //         {
//         //             cout << "thread id:" << this_thread::get_id() << " i:" << i << endl;
//         //             this_thread::sleep_for(chrono::seconds(1));
//         //             cout << "thread id:" << this_thread::get_id() << "hello  world" << endl;
//         //             return i * i;
//         //         }));
//         double b = 5.5;
//         resultList.emplace_back(
//             pool.addTask(_add, i, b));
//     }
//     for (auto &&result : resultList)
//     {
//         cout << "result:" << result.get() << endl;
//     }
//     while (1)
//         ;
// }

#include "./include/GlobalFuntion.h"

int main()
{
    xcfj::test_for_log_and_mutiThread();
    return 0;
}
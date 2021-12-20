#include <mutex>
#include <memory>
#include <iostream>
#include "config.h"

namespace std
{
    template <typename klassType>
    class SingleTon
    {
        using smart_ptr = shared_ptr<klassType>;
        using ptr = klassType *;

    public:
        static smart_ptr &getInstance();
        static size_t useCount()
        {
            return g_instance.use_count();
        }
        static ptr get()
        {
            return g_instance.get();
        }
        virtual ~SingleTon()
        {
#ifdef DEBUG
            cout << "destory singleton" << endl;
#endif
        };

    private:
        static smart_ptr g_instance;
        static mutex g_mutex;

    protected:
        SingleTon();
        SingleTon(const SingleTon &) = delete;
        SingleTon &operator=(const SingleTon &) = delete;
    };
    template <typename klassType>
    SingleTon<klassType>::SingleTon()
    {
#ifdef DEBUG
        cout << "construct singleton" << endl;
#endif
    }
    /* 初始化静态成员 */
    template <typename klassType>
    mutex SingleTon<klassType>::g_mutex;
    template <typename klassType>
    shared_ptr<klassType> SingleTon<klassType>::g_instance(new klassType,
                                                           [](klassType *ptrInFace)
                                                           {
                                                               if (g_instance.get() != nullptr)
                                                               {
                                                                   delete ptrInFace;
                                                                   ptrInFace = nullptr;
#ifdef DEBUG
                                                                   cout << "deletor end" << endl;
#endif
                                                               }
                                                           });
    /* 获取单例对象 */
    template <typename klassType>
    shared_ptr<klassType> &SingleTon<klassType>::getInstance()
    {
        {
            lock_guard<mutex> lock(g_mutex);
            if (g_instance.get() == nullptr)
            {
                g_instance.reset(new klassType);
            }
        }
        return g_instance;
    }
} // namespace std
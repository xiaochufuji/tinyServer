#include <mutex>
#include <memory>
#include <iostream>
using namespace std;
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
        virtual ~SingleTon(){
            // cout << "destory singleton" << endl;
        };

    private:
        static smart_ptr g_instance;

    protected:
        SingleTon();
        SingleTon(const SingleTon &) = delete;
        SingleTon &operator=(const SingleTon &) = delete;
    };
    template <typename klassType>
    SingleTon<klassType>::SingleTon()
    {
        // cout << "construct singleton" << endl;
    }
    /* 初始化静态成员 */
    template <typename klassType>
    shared_ptr<klassType> SingleTon<klassType>::g_instance(new klassType,
                                                           [](klassType *ptrInFace)
                                                           {
                                                               if (g_instance.get() != nullptr)
                                                               {
                                                                   delete ptrInFace;
                                                                   ptrInFace = nullptr;
                                                                   // cout << "deletor end" << endl;
                                                               }
                                                           });
    /* 获取单例对象 */
    template <typename klassType>
    shared_ptr<klassType> &SingleTon<klassType>::getInstance()
    {

        lock_guard<mutex> lock();
        if (g_instance.get() == nullptr)
        {
            g_instance.reset(new klassType);
        }
        return g_instance;
    }
} // namespace std
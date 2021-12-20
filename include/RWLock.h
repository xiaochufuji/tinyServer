#pragma once
#include <mutex>
#include <pthread.h>

namespace std
{
    class RWLock
    {
    public:
        RWLock() { pthread_rwlock_init(&m_lock, nullptr); }
        ~RWLock() { pthread_rwlock_destroy(&m_lock); }
        RWLock(const RWLock &) = delete;
        RWLock &operator=(const RWLock &) = delete;
        RWLock(const RWLock &&oldRWLock) = delete;
        RWLock &operator=(const RWLock &&) = delete;

    public:
        int rdLock() { return pthread_rwlock_rdlock(&m_lock); }
        int wrLock() { return pthread_rwlock_wrlock(&m_lock); }
        int unlock() { return pthread_rwlock_unlock(&m_lock); }

    private:
        pthread_rwlock_t m_lock;
    };
    /* 读写锁的自锁解锁 */
    class lock_guard_RD
    {
    public:
        lock_guard_RD(RWLock &lock) : m_lock(lock) { m_lock.rdLock(); }
        ~lock_guard_RD() { m_lock.unlock(); }
        lock_guard_RD(const lock_guard_RD &) = delete;
        lock_guard_RD &operator=(const lock_guard_RD &) = delete;

    private:
        RWLock &m_lock;
    };
    class lock_guard_WR
    {
    public:
        lock_guard_WR(RWLock &lock) : m_lock(lock) { m_lock.wrLock(); }
        ~lock_guard_WR() { m_lock.unlock(); }
        lock_guard_WR(const lock_guard_WR &) = delete;
        lock_guard_WR &operator=(const lock_guard_WR &) = delete;

    private:
        RWLock &m_lock;
    };
}
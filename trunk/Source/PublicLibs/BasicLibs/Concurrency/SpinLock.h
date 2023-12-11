/* SpinLock.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 10/22/2016
 *  Last Modified   : 10/22/2016
 * 
 */

#pragma once
#ifndef ymp_Concurrency_SpinLock_H
#define ymp_Concurrency_SpinLock_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <atomic>
#include "PublicLibs/CompilerSettings.h"
#include "PublicLibs/BasicLibs/Concurrency/SpinPause.h"
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class SpinLock{
public:
    YM_FORCE_INLINE SpinLock() : m_locked(false) {}

    YM_FORCE_INLINE void spin_acquire(){
        bool state = false;
        if (m_locked.compare_exchange_weak(state, true)){
            return;
        }
        do{
            Intrinsics::spin_pause();
            state = false;
        }while (m_locked.load(std::memory_order_acquire) || !m_locked.compare_exchange_weak(state, true));
    }

#if 0
    YM_FORCE_INLINE void sleep_acquire(){
        bool state = false;
        if (m_locked.compare_exchange_weak(state, true)){
            return;
        }
        do{
            Intrinsics::sleep_pause();
            state = false;
        }while (m_locked.load(std::memory_order_acquire) || !m_locked.compare_exchange_weak(state, true));
    }
#endif

    YM_FORCE_INLINE void unlock(){
        m_locked.store(false, std::memory_order_release);
    }

private:
    std::atomic<bool> m_locked;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class SpinLockGuard{
public:
    SpinLockGuard(const SpinLockGuard&) = delete;
    void operator=(const SpinLockGuard&) = delete;

    YM_FORCE_INLINE SpinLockGuard(SpinLock& lock)
        : m_lock(lock)
    {
        lock.spin_acquire();
    }
    YM_FORCE_INLINE ~SpinLockGuard(){
        m_lock.unlock();
    }

private:
    SpinLock& m_lock;
};
#if 0
class SleepLockGuard{
public:
    SleepLockGuard(const SleepLockGuard&) = delete;
    void operator=(const SleepLockGuard&) = delete;

    YM_FORCE_INLINE SleepLockGuard(SpinLock& lock)
        : m_lock(lock)
    {
        lock.sleep_acquire();
    }
    YM_FORCE_INLINE ~SleepLockGuard(){
        m_lock.unlock();
    }

private:
    SpinLock& m_lock;
};
#endif
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif

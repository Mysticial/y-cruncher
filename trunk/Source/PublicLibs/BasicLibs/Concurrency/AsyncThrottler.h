/* AsyncThrottler.h
 * 
 *  Author          : Alexander J. Yee
 *  Date Created    : 10/15/2023
 *  Last Modified   : 10/15/2023
 * 
 *      Throttle certain async operations where bad things may happen if you
 *  try to do too many at once. (such as async I/O)
 * 
 */

#pragma once
#ifndef ymp_AsyncThrottler_H
#define ymp_AsyncThrottler_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include <atomic>
#include <mutex>
#include <condition_variable>
namespace ymp{
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
class AsyncThrottler{
public:
    AsyncThrottler(upL_t max_active)
        : m_max_active(max_active)
        , m_active(0)
    {}

    //  Attempt to run the lambda using the provided throttler.
    //  If too many are already in flight, it will wait until there is room.
    //  If the operation fails (lambda returns false), it will wait and retry.
    template <typename Lambda>
    void run(Lambda&& lambda){
        while (true){
            {
                std::unique_lock<std::mutex> lg(m_lock);
                m_cv.wait(lg, [=]{ return m_active < m_max_active; });
                m_active++;
            }

            bool success;
            try{
                success = lambda();
            }catch (...){
                std::unique_lock<std::mutex> lg(m_lock);
                m_active--;
                m_cv.notify_all();
                throw;
            }

            std::unique_lock<std::mutex> lg(m_lock);
            upL_t active = --m_active;

            if (success){
                m_cv.notify_all();
                return;
            }

            if (active > 0){
                m_cv.wait(lg, [=]{ return m_active < active; });
            }
        }
    }

private:
    upL_t m_max_active;
    upL_t m_active;
    std::mutex m_lock;
    std::condition_variable m_cv;
};
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
}
#endif

//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

#ifndef rtosim_ThreadPoolJobs_h
#define rtosim_ThreadPoolJobs_h

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace rtosim {

    template <typename T>
    class ThreadPoolJobs
    {
    public:
        ThreadPoolJobs() = default;
        ThreadPoolJobs(const ThreadPoolJobs&) = delete;            // disable copying
        ThreadPoolJobs& operator=(const ThreadPoolJobs&) = delete; // disable assignment
        T pop();
        size_t size();
        void pop(T& item);
        T front();
        void front(T& item);
        void push(const T& item);

    private:
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}

#include "ThreadPoolJobs.cpp"
#endif

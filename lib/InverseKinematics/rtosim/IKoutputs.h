//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
//            Claudio Pizzolato - May 2016
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

#ifndef rtosim_IKoutputs_h
#define rtosim_IKoutputs_h

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace rtosim {

    template <typename T>
    class IKoutputs
    {
    public:
        IKoutputs() = default;
        IKoutputs(const IKoutputs&) = delete;            // disable copying
        IKoutputs& operator=(const IKoutputs&) = delete; // disable assignment
        T pop(double currentTime);
        T pop();
        void pop(T& item);
        void push(const T& item);

    private:

        struct OrderByTime
        {
            bool operator() (T const &a, T const &b) { return a.time > b.time; }
        };

        std::priority_queue<T, std::vector<T>, OrderByTime > queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}

#include "IKoutputs.cpp"
#endif

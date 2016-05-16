//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

#ifndef rtosim_TimeSequence
#define rtosim_TimeSequence

#include "rtosim/concurrency/Queue.h"
#include <string>
#include <thread>
#include <queue>

namespace rtosim{

    class TimeSequence
    {
    public:
        TimeSequence() = default;
        TimeSequence(const TimeSequence&) = delete;            // disable copying
        TimeSequence& operator=(const TimeSequence&) = delete; // disable assignment
        double pop();
        void push(const double& item);

    private:
        std::queue<double> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}

#endif
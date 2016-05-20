#ifndef rtosim_FrameCounter_h
#define rtosim_FrameCounter_h

#include "rtosim/StopWatch.h"
#include <chrono>
#include <iostream>

namespace rtosim{

    template <typename T>
    class FrameCounter {
    public:
        FrameCounter(FrameCounter<T>&) = delete;
        FrameCounter(T& queueIn, const std::string& name = "");
        virtual ~FrameCounter() {}
        StopWatch getProcessingTimes() const;
        void operator()();
    private:
        T& queueIn_;
        StopWatch stopWatch_;
    };
}

#include "FrameCounter.cpp"
#endif

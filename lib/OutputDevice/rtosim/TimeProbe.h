#ifndef rtosim_TimeProbe_h
#define rtosim_TimeProbe_h

#include <ctime>
#include <chrono>
#include <list>
//calculates the time that takes a frame to 
//travel from one queue to the other

namespace rtosim {

    template<typename Q>
    class Probe {
    public:
        Probe(Q& queue);
        void operator()();
        std::list<double> getWallClockTimes() const;
        std::list<double> getCpuClockTimes() const;
        std::list<double> getTimesFromFrames() const;
        void initialise();
    private:
        void logCurrentTime();
        Q& queue_;
        
        std::chrono::system_clock::time_point t_initialTimePoint_;
        std::list<double> t_frameProcessingTime_;

        std::clock_t c_initialTimePoint_;
        std::list<double> c_frameProcessingTime_;

        std::list<double> timesFromFrames_;

    };

    template<typename Qin, typename Qout>
    class TimeProbe {
    public:
        TimeProbe() = delete;
        TimeProbe(const TimeProbe<Qin, Qout>&) = delete;
        TimeProbe(Qin& queueIn, Qout& queueOut);
        void operator()();

    private:
        Qin& queueIn_;
        Qout& queueOut_;
        StopWatch stopWatch_;
    };

}

#include "TimeProbe.cpp"
#endif
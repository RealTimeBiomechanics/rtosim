#include "rtosim/EndOfData.h"
#include "rtosim/QueuesSync.h"
#include <chrono>
using std::chrono::system_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

namespace rtosim {

    template<typename Q>
    Probe<Q>::Probe(Q& queue) :
        queue_(&queue) {
    
        initialise()
    }


    template<typename Q>
    void Probe<Q>::initialise() {

        t_initialTimePoint_ = high_resolution_clock::now();
        c_initialTimePoint_ = std::clock();
    }

    template<typename Q>
    void Probe<Q>::logCurrentTime() {


        system_clock::time_point t_newTimePoint(high_resolution_clock::now());
        std::clock_t c_newTimePoint(std::clock());

        //using system clock
        double t_duration(duration_cast<std::chrono::nanoseconds>(t_newTimePoint - t_initialTimePoint_).count());
        t_frameProcessingTime_.push_back(t_duration*1e-9);

        //using cpu clock
        double c_duration(static_cast<double>(c_newTimePoint - c_initialTimePoint_) / CLOCKS_PER_SEC);
        c_frameProcessingTime_.push_back(c_duration);

    }

    template<typename Q>
    std::list<double> Probe<Q>::getWallClockTimes() const {
    
        return t_frameProcessingTime_;
    }

    template<typename Q>
    std::list<double> Probe<Q>::getCpuClockTimes() const{
        
        return c_frameProcessingTime_;
    }

    template<typename Q>
    std::list<double> Probe<Q>::getTimesFromFramest() const{
        
        return timesFromFrames_;
    }

    template<typename Q>
    void Probe<Q>::operator()() {

        queue_.subscribe();
        auto frame(queue_.pop());
        bool runCondition(!EndOfData::isEod(frame));
        initialise(); //reset the initial time point
        while (runCondition) {
            auto frame(queue_.pop());
            logCurrentTime();
            timesFromFrames_.emplace_back(frame.time);
            runCondition = !EndOfData::isEod(frame);
        }

    }


    template<typename Qin, typename Qout>
    TimeProbe<Qin, Qout>::TimeProbe(Qin& queueIn, Qout& queueOut) :
        queueIn_(&queueIn),
        queueOut_(&queueOut) {
    }


    template<typename Qin, typename Qout>
    void TimeProbe<Qin, Qout>::operator()() {

        Probe pIn(queueIn_);
        Probe pOut(queueOut_);

        QueuesSync::launchThreads(pIn, pOut);
       
    }

}
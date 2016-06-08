#ifndef rtosim_TimeProbe_h
#define rtosim_TimeProbe_h

#include <ctime>
#include <chrono>
#include <list>
#include <vector>
#include <tuple>
#include <ostream>
//calculates the time that takes a frame to 
//travel from one queue to the other

namespace rtosim {

    //tuple compare is from http://stackoverflow.com/questions/23030267/custom-sorting-a-vector-of-tuples
    template<int M, template<typename> class F = std::less>
    struct TupleCompare
    {
        template<typename T>
        bool operator()(T const &t1, T const &t2)
        {
            return F<typename std::tuple_element<M, T>::type>()(std::get<M>(t1), std::get<M>(t2));
        }
    };

    template<typename T>
    class TimeData;

    template<typename T>
    TimeData<T> operator-(TimeData<T> lhs, const TimeData<T>& rhs);

    template<typename T>
    std::ostream& operator <<(std::ostream& os, const TimeData<T>& fs);


    template<typename T>
    class TimeData {
    
    public:
        TimeData();
        void setTime(const T& key, double measuredTime);
        std::vector<double> getTimes() const;
        std::vector<T> getKeys() const;
        double getTime(const T& key) const;
        double getMean() const;
        double getStd() const;
        double getMedian() const;
        double getMin() const;
        double getMax() const;
        TimeData<T>& operator-=(const TimeData<T>& rhs);
        friend std::ostream& operator << <>(std::ostream& os, const TimeData<T>& fs);
    private:

        using TimeDataValues = std::vector < std::tuple<T, double>>;
        TimeDataValues data_;
    };

    template<typename Q>
    class TimeProbe {
    public:
        TimeProbe(Q& queue);
        void operator()();
        TimeData<double> getWallClockTimes() const;
        TimeData<double> getCpuClockTimes() const;
    
        void initialise();
    private:
        //uses frameTime as key and logs the cpu and wall clocks
        void logCurrentTime(double frameTime);
        Q& queue_;
        
        std::chrono::system_clock::time_point t_initialTimePoint_;
        TimeData<double> t_frameProcessingTimes_;

        std::clock_t c_initialTimePoint_;
        TimeData<double> c_frameProcessingTimes_;

    };

    template<typename Qin, typename Qout>
    class TimeDifference {
    public:
        TimeDifference() = delete;
        TimeDifference(const TimeDifference<Qin, Qout>&) = delete;
        TimeDifference(Qin& queueIn, Qout& queueOut);
        void operator()();
        TimeData<double> getWallClockDifference() const;
        TimeData<double> getCpuClockDifference() const;

    private:
        Qin& queueIn_;
        Qout& queueOut_;
        TimeData<double> wallClockDifference_, cpuClockDifference_;
    };

}

#include "TimeDifference.cpp"
#endif
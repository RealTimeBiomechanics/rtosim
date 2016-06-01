#ifndef rtosim_StopWatch_h
#define rtosim_StopWatch_h

#include <thread>
#include <list>
#include <string>
#include <ctime>

namespace rtosim {

    template<typename T>
    typename T::value_type getStd(const T& values);
    template<typename T>
    typename T::value_type getMean(const T& values);

    class StopWatch;

    std::ostream& operator <<(std::ostream& os, const StopWatch& fs);
    StopWatch operator+(StopWatch lhs, const StopWatch& rhs);

    class StopWatch{
    public:
        using Double = double;
        StopWatch();
        StopWatch(const std::string& name);
        std::thread::id getId() { return id_; }
        void init();
        void log();
        unsigned size() const;
        Double getMinProcessingTimeWallClock() const;
        Double getMaxProcessingTimeWallClock() const;
        Double getTotalProcessingTimeWallClock() const;
        Double getSumProcessingTimesWallClock() const;
        Double getMeanProcessingTimeWallClock() const;
        Double getStdProcessingTimeWallClock() const;
        Double getMedianProcessingTimeWallClock() const;

        Double getMinProcessingTimeCpuClock() const;
        Double getMaxProcessingTimeCpuClock() const;
        Double getTotalProcessingTimeCpuClock() const;
        Double getSumProcessingTimesCpuClock() const;
        Double getMeanProcessingTimeCpuClock() const;
        Double getStdProcessingTimeCpuClock() const;
        Double getMedianProcessingTimeCpuClock() const;

        friend std::ostream& operator <<(std::ostream& os, const StopWatch& fs);
        StopWatch& operator+=(const StopWatch& rhs);
        void print(const std::string& outputDir);

    private:
        Double getMedian(const std::list<Double>& v) const;
        std::chrono::system_clock::time_point t_timePoint_, t_initialTimePoint_, t_finalTimePoint_;
        std::list<Double> t_frameProcessingTime_;

        std::clock_t c_timePoint_, c_initialTimePoint_, c_finalTimePoint_;
        std::list<Double> c_frameProcessingTime_;

        std::thread::id id_;
        std::string name_;
    };
}



#endif
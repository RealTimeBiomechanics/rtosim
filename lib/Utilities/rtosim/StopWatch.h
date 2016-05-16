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
        StopWatch();
        StopWatch(const std::string& name);
        std::thread::id getId() { return id_; }
        void init();
        void log();
        unsigned size() const;
        double getMinProcessingTimeWallClock() const;
        double getMaxProcessingTimeWallClock() const;
        double getTotalProcessingTimeWallClock() const;
        double getSumProcessingTimesWallClock() const;
        double getMeanProcessingTimeWallClock() const;
        double getStdProcessingTimeWallClock() const;

        double getMinProcessingTimeCpuClock() const;
        double getMaxProcessingTimeCpuClock() const;
        double getTotalProcessingTimeCpuClock() const;
        double getSumProcessingTimesCpuClock() const;
        double getMeanProcessingTimeCpuClock() const;
        double getStdProcessingTimeCpuClock() const;

        friend std::ostream& operator <<(std::ostream& os, const StopWatch& fs);
        StopWatch& operator+=(const StopWatch& rhs);
        void print(const std::string& outputDir);

    private:

        std::chrono::system_clock::time_point t_timePoint_, t_initialTimePoint_, t_finalTimePoint_;
        std::list<double> t_frameProcessingTime_;

        std::clock_t c_timePoint_, c_initialTimePoint_, c_finalTimePoint_;
        std::list<double> c_frameProcessingTime_;

        std::thread::id id_;
        std::string name_;
    };
}

#endif
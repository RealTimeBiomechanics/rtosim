#include "rtosim/StopWatch.h"
#include "rtosim/FileSystem.h"

#include <chrono>
using std::chrono::system_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
#include <numeric>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace rtosim {

    StopWatch::StopWatch() :
        name_("stop_watch")
    {
        init();
    }

    StopWatch::StopWatch(const std::string& name) :
        name_(name)
    {
        init();
    }

    void StopWatch::init() {
        id_ = std::this_thread::get_id();
        t_initialTimePoint_ = t_finalTimePoint_ = t_timePoint_ = high_resolution_clock::now();
        c_initialTimePoint_ = c_finalTimePoint_ = c_timePoint_ = std::clock();
    }

    void StopWatch::log() {

        system_clock::time_point t_newTimePoint(high_resolution_clock::now());
        std::clock_t c_newTimePoint(std::clock());

        //using system clock
        auto t_duration(duration_cast<std::chrono::nanoseconds>(t_newTimePoint - t_timePoint_).count());
        t_timePoint_ = t_newTimePoint;
        t_finalTimePoint_ = t_timePoint_;
        double t_processingTime(t_duration*1e-9);
        t_frameProcessingTime_.push_back(t_processingTime);

        //using cpu clock
        double c_duration(static_cast<double>(c_newTimePoint - c_timePoint_) / CLOCKS_PER_SEC);
        c_timePoint_ = c_newTimePoint;
        c_finalTimePoint_ = c_timePoint_;
        c_frameProcessingTime_.push_back(c_duration);
    }

    unsigned StopWatch::size() const {

        return t_frameProcessingTime_.size();
    }

    double StopWatch::getMinProcessingTimeWallClock() const {

        auto it = std::min_element(t_frameProcessingTime_.begin(), t_frameProcessingTime_.end());
        return *it;
    }

    double StopWatch::getMinProcessingTimeCpuClock() const {

        auto it = std::min_element(c_frameProcessingTime_.begin(), c_frameProcessingTime_.end());
        return *it;
    }

    double StopWatch::getMaxProcessingTimeWallClock() const {

        auto it = std::max_element(t_frameProcessingTime_.cbegin(), t_frameProcessingTime_.cend());
        return *it;
    }

    double StopWatch::getMaxProcessingTimeCpuClock() const {

        auto it = std::max_element(c_frameProcessingTime_.cbegin(), c_frameProcessingTime_.cend());
        return *it;
    }

    double StopWatch::getMeanProcessingTimeWallClock() const {

        return getMean(t_frameProcessingTime_);
    }

    double StopWatch::getMeanProcessingTimeCpuClock() const {

        return getMean(c_frameProcessingTime_);
    }

    double StopWatch::getStdProcessingTimeWallClock() const {

        return  getStd(t_frameProcessingTime_);
    }

    double StopWatch::getStdProcessingTimeCpuClock() const {

        return  getStd(c_frameProcessingTime_);
    }

    double StopWatch::getTotalProcessingTimeWallClock() const {

        double duration(duration_cast<std::chrono::nanoseconds>(t_finalTimePoint_ - t_initialTimePoint_).count());
        return duration*1e-9;
    }

    double StopWatch::getTotalProcessingTimeCpuClock() const {

        return static_cast<double>(c_finalTimePoint_ - c_initialTimePoint_) / CLOCKS_PER_SEC;
    }

    double StopWatch::getSumProcessingTimesWallClock() const {

        return getMeanProcessingTimeWallClock()*size();
    }

    double StopWatch::getSumProcessingTimesCpuClock() const {

        return getMeanProcessingTimeCpuClock()*size();
    }

    StopWatch& StopWatch::operator+=(const StopWatch& rhs)
    {
        t_initialTimePoint_ = t_finalTimePoint_ = t_timePoint_ = high_resolution_clock::now();
        this->t_frameProcessingTime_.insert(this->t_frameProcessingTime_.end(), rhs.t_frameProcessingTime_.begin(), rhs.t_frameProcessingTime_.end());

        c_initialTimePoint_ = c_finalTimePoint_ = c_timePoint_ = std::clock();
        this->c_frameProcessingTime_.insert(this->c_frameProcessingTime_.end(), rhs.c_frameProcessingTime_.begin(), rhs.c_frameProcessingTime_.end());

        return *this;
    }

    StopWatch operator+(StopWatch lhs, const StopWatch& rhs) {

        return lhs += rhs;
    }

    void StopWatch::print(const std::string& outputDir) {

        if (!FileSystem::createDirectory(outputDir))
            std::cout << "failed ";
        std::cout << "printing " << name_ << " in " << outputDir << std::endl;
        std::ofstream outF(outputDir + "/" + name_ + ".txt");
        outF << *this;
        outF.close();
    }

    std::ostream& operator <<(std::ostream& os, const StopWatch& fs) {

        os << fs.name_ << " (thread#" << fs.id_ << ")" << std::endl;
        os << "frames: " << fs.size() << std::endl;
        os << "wall clock time\n";
        os << "-total: " << fs.getTotalProcessingTimeWallClock() << std::endl;
        os << "-sum:   " << fs.getSumProcessingTimesWallClock() << std::endl;
        os << "-mean:  " << fs.getMeanProcessingTimeWallClock() << std::endl;
        os << "-std:   " << fs.getStdProcessingTimeWallClock() << std::endl;
        os << "-min:   " << fs.getMinProcessingTimeWallClock() << std::endl;
        os << "-max:   " << fs.getMaxProcessingTimeWallClock() << std::endl;
        os << "cpu clock time\n";
        os << "-total: " << fs.getTotalProcessingTimeCpuClock() << std::endl;
        os << "-sum:   " << fs.getSumProcessingTimesCpuClock() << std::endl;
        os << "-mean:  " << fs.getMeanProcessingTimeCpuClock() << std::endl;
        os << "-std:   " << fs.getStdProcessingTimeCpuClock() << std::endl;
        os << "-min:   " << fs.getMinProcessingTimeCpuClock() << std::endl;
        os << "-max:   " << fs.getMaxProcessingTimeCpuClock() << std::endl;

        return os;
    }

    template<typename T>
    typename T::value_type getMean(const T& values) {

        typename T::value_type sum(std::accumulate(values.cbegin(), values.cend(), typename T::value_type{ 0 }));
        return (sum / static_cast<typename T::value_type>(values.size()));
    }

    template<typename T>
    typename T::value_type getStd(const T& values) {

        auto mean(getMean(values));

        typename T::value_type e(0);
        for (auto v : values)
            e += (v - mean)*(v - mean);
        e /= values.size();
        return std::sqrt(e);
    }
}
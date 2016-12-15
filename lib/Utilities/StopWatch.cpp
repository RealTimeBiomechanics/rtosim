/* -------------------------------------------------------------------------- *
 * Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                          *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at:                                   *
 * http://www.apache.org/licenses/LICENSE-2.0                                 *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#include "rtosim/StopWatch.h"
#include "rtosim/FileSystem.h"

#include <chrono>
using std::chrono::system_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

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
        t_initialTimePoint_ = t_finalTimePoint_ = t_timePoint_ = system_clock::now();
        c_initialTimePoint_ = c_finalTimePoint_ = c_timePoint_ = std::clock();
    }

    void StopWatch::log() {

        system_clock::time_point t_newTimePoint(system_clock::now());
        std::clock_t c_newTimePoint(std::clock());

        //using system clock
        Double t_duration(duration_cast<std::chrono::nanoseconds>(t_newTimePoint - t_timePoint_).count());
        t_timePoint_ = t_newTimePoint;
        t_finalTimePoint_ = t_timePoint_;
        Double t_processingTime(t_duration*1e-9);
        t_frameProcessingTime_.push_back(t_processingTime);

        //using cpu clock
        Double c_duration(static_cast<Double>(c_newTimePoint - c_timePoint_) / CLOCKS_PER_SEC);
        c_timePoint_ = c_newTimePoint;
        c_finalTimePoint_ = c_timePoint_;
        c_frameProcessingTime_.push_back(c_duration);
    }

    unsigned StopWatch::size() const {

        return t_frameProcessingTime_.size();
    }

    StopWatch::Double StopWatch::getMinProcessingTimeWallClock() const {

        auto it = std::min_element(t_frameProcessingTime_.begin(), t_frameProcessingTime_.end());
        return *it;
    }

    StopWatch::Double StopWatch::getMinProcessingTimeCpuClock() const {

        auto it = std::min_element(c_frameProcessingTime_.begin(), c_frameProcessingTime_.end());
        return *it;
    }

    StopWatch::Double StopWatch::getMaxProcessingTimeWallClock() const {

        auto it = std::max_element(t_frameProcessingTime_.cbegin(), t_frameProcessingTime_.cend());
        return *it;
    }

    StopWatch::Double StopWatch::getMaxProcessingTimeCpuClock() const {

        auto it = std::max_element(c_frameProcessingTime_.cbegin(), c_frameProcessingTime_.cend());
        return *it;
    }

    StopWatch::Double StopWatch::getMeanProcessingTimeWallClock() const {

        return getMean(t_frameProcessingTime_);
    }

    StopWatch::Double StopWatch::getMeanProcessingTimeCpuClock() const {

        return getMean(c_frameProcessingTime_);
    }

    StopWatch::Double StopWatch::getStdProcessingTimeWallClock() const {

        return  getStd(t_frameProcessingTime_);
    }

    StopWatch::Double StopWatch::getStdProcessingTimeCpuClock() const {

        return  getStd(c_frameProcessingTime_);
    }


    StopWatch::Double StopWatch::getMedian(const std::list<Double>& l) const {

        std::size_t n(l.size() / 2);
        std::vector<Double> temp(l.begin(), l.end());
        std::nth_element(temp.begin(), std::next(temp.begin(), n), temp.end());
        return temp.at(n-1);
    }

    StopWatch::Double StopWatch::getMedianProcessingTimeWallClock() const {

        return getMedian(t_frameProcessingTime_);
    }

    StopWatch::Double StopWatch::getMedianProcessingTimeCpuClock() const {

        return getMedian(c_frameProcessingTime_);
    }

    StopWatch::Double StopWatch::getTotalProcessingTimeWallClock() const {

        Double duration(duration_cast<std::chrono::nanoseconds>(t_finalTimePoint_ - t_initialTimePoint_).count());
        return duration*1e-9;
    }

    StopWatch::Double StopWatch::getTotalProcessingTimeCpuClock() const {

        return static_cast<Double>(c_finalTimePoint_ - c_initialTimePoint_) / CLOCKS_PER_SEC;
    }

    StopWatch::Double StopWatch::getSumProcessingTimesWallClock() const {

        return getMeanProcessingTimeWallClock()*size();
    }

    StopWatch::Double StopWatch::getSumProcessingTimesCpuClock() const {

        return getMeanProcessingTimeCpuClock()*size();
    }

    StopWatch& StopWatch::operator+=(const StopWatch& rhs)
    {
        t_initialTimePoint_ = t_finalTimePoint_ = t_timePoint_ = system_clock::now();
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
        #ifdef _WIN32
        os << "WARNING: times may be wrong on Windows" << std::endl;
        #endif
        os << fs.name_ << " (thread#" << fs.id_ << ")" << std::endl;
        os << "frames: " << fs.size() << std::endl;
        os << "wall clock time\n";
        os << "-total:  " << fs.getTotalProcessingTimeWallClock() << std::endl;
        os << "-sum:    " << fs.getSumProcessingTimesWallClock() << std::endl;
        os << "-mean:   " << fs.getMeanProcessingTimeWallClock() << std::endl;
        os << "-std:    " << fs.getStdProcessingTimeWallClock() << std::endl;
        os << "-min:    " << fs.getMinProcessingTimeWallClock() << std::endl;
        os << "-max:    " << fs.getMaxProcessingTimeWallClock() << std::endl;
        os << "-median: " << fs.getMedianProcessingTimeWallClock() << std::endl;
        os << "cpu clock time\n";
        os << "-total:  " << fs.getTotalProcessingTimeCpuClock() << std::endl;
        os << "-sum:    " << fs.getSumProcessingTimesCpuClock() << std::endl;
        os << "-mean:   " << fs.getMeanProcessingTimeCpuClock() << std::endl;
        os << "-std:    " << fs.getStdProcessingTimeCpuClock() << std::endl;
        os << "-min:    " << fs.getMinProcessingTimeCpuClock() << std::endl;
        os << "-max:    " << fs.getMaxProcessingTimeCpuClock() << std::endl;
        os << "-median: " << fs.getMedianProcessingTimeCpuClock() << std::endl;

        return os;
    }

}
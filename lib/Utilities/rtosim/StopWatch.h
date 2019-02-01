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

#ifndef rtosim_StopWatch_h
#define rtosim_StopWatch_h

#include <thread>
#include <list>
#include <string>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <cmath>

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
        std::list<Double> getAllProcessingTimeWallClock() const;

        Double getMinProcessingTimeCpuClock() const;
        Double getMaxProcessingTimeCpuClock() const;
        Double getTotalProcessingTimeCpuClock() const;
        Double getSumProcessingTimesCpuClock() const;
        Double getMeanProcessingTimeCpuClock() const;
        Double getStdProcessingTimeCpuClock() const;
        Double getMedianProcessingTimeCpuClock() const;
        std::list<Double> getAllProcessingTimeCpuClock() const;

        friend std::ostream& operator <<(std::ostream& os, const StopWatch& fs);
        StopWatch& operator+=(const StopWatch& rhs);
        void print(const std::string& outputDir);

    private:
        Double getMedian(const std::list<Double>& v) const; //move it as free function
        std::chrono::steady_clock::time_point t_timePoint_, t_initialTimePoint_, t_finalTimePoint_;
        std::list<Double> t_frameProcessingTime_;

        std::clock_t c_timePoint_, c_initialTimePoint_, c_finalTimePoint_;
        std::list<Double> c_frameProcessingTime_;

        std::thread::id id_;
        std::string name_;
    };

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



#endif

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

#include "rtosim/EndOfData.h"
#include "rtosim/QueuesSync.h"
#include <chrono>
using std::chrono::system_clock;
using std::chrono::duration_cast;
#include <algorithm>
#include <numeric>
#include <iterator>
#include <list>
#include <stdexcept>
#include <fstream>

namespace rtosim {

    template<typename T>
    TimeData<T>::TimeData() {}

    template<typename T>
    TimeData<T>& TimeData<T>::operator-=(const TimeData<T>& rhs) {

        //sort the data because we want to track single frames, which are not necessary in order
        typename TimeData<T>::TimeDataValues sortedDataLhs(this->data_), sortedDataRhs(rhs.data_), newData;
        std::sort(std::begin(sortedDataLhs), std::begin(sortedDataLhs), TupleCompare<0>());
        std::sort(std::begin(sortedDataRhs), std::begin(sortedDataRhs), TupleCompare<0>());

        //the following is sismilar to std::set_intersection but
        //saves the difference of the times
        auto first1(std::begin(sortedDataLhs));
        auto last1(std::end(sortedDataLhs));
        auto first2(std::begin(sortedDataRhs));
        auto last2(std::end(sortedDataRhs));

        while ((first1 != last1) && (first2 != last2)) {
            if (TupleCompare<0>()(*first1, *first2)) {
                std::advance(first1, 1);
            }
            else  {
                if (!TupleCompare<0>()(*first2, *first1)) {
                    T key(std::get<0>(*first2));
                    double diff(std::get<1>(*first1) - std::get<1>(*first2));
                    newData.emplace_back(std::make_tuple(key, diff));
                    std::advance(first1, 1);
                }
                std::advance(first2, 1);
            }
        }
        this->data_ = newData;
        return *this;
    }


    template<typename T>
    TimeData<T> operator-(TimeData<T> lhs, const TimeData<T>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    template<typename T>
    std::size_t TimeData<T>::size() const {

        return data_.size();
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const TimeData<T>& rhs) {

        
        os << "#frames    : " << rhs.size() << std::endl;
        os << "mean (s)   : " << rhs.getMean() << std::endl;
        os << "std (s)    : " << rhs.getStd()  << std::endl;
        os << "median (s) : " << rhs.getMedian()  << std::endl;
        os << "min (s)    : " << rhs.getMin() << std::endl;
        os << "max (s)    : " << rhs.getMax()  << std::endl;
        
        /*
        os << "Key\t\tTime" << std::endl;
        for (auto& e : rhs.data_)
            os << std::get<0>(e) << "\t" << std::get<1>(e) << std::endl;
          */  
        return os;
    }

    template<typename T>
    void TimeData<T>::print(const std::string& filename) const {

        std::ofstream outF(filename);
        if (outF.is_open())
            outF << *this;
    }

    template<typename T>
    void TimeData<T>::setTime(const T& key, double time) {

        data_.emplace_back(std::make_tuple(key, time));

    }


    template<typename T>
    std::vector<double> TimeData<T>::getTimes() const {
    
        std::vector<double> times;
        for (auto& e : data_)
            times.emplace_back(std::get<1>(e));
        return times;
    
    }
    
    template<typename T>
    std::vector<T> TimeData<T>::getKeys() const {
    
        std::vector<T> keys;
        for (auto& e : data_)
            keys.emplace_back(std::get<0>(e));
        return keys;
    }
    
    template<typename T>
    double TimeData<T>::getTime(const T& key) const {
    
        auto it(std::find_if(std::begin(data_), std::end(data_), key, TupleCompare<0, std::equal_to>()));
        if (it == std::end(data_))
            throw std::invalid_argument("key " + std::to_string(key) + " not found");
        return *it;
    }
    
    template<typename T>
    double TimeData<T>::getMean() const {
    
        auto values(getTimes());
        double sum(std::accumulate(values.cbegin(), values.cend(), 0.));
        return (sum / static_cast<double>(values.size()));

    }
    
    template<typename T>
    double TimeData<T>::getStd() const {
        
        auto values(getTimes());
        auto mean(getMean());
        double e(0);
        for (auto v : values)
            e += (v - mean)*(v - mean);
        e /= values.size();
        return std::sqrt(e);
    }
    
    template<typename T>
    double TimeData<T>::getMedian() const {
        
        auto values(getTimes());
        std::size_t n(values.size() / 2);
        std::nth_element(std::begin(values), std::next(std::begin(values), n), std::end(values));
        return values.at(n - 1);
    }

    template<typename T>
    double TimeData<T>::getMin() const {

        auto it(std::min_element(std::begin(data_), std::end(data_), TupleCompare<1>()));
        return std::get<1>(*it);
    }

    template<typename T>
    double TimeData<T>::getMax() const {

        auto it(std::max_element(std::begin(data_), std::end(data_), TupleCompare<1>()));
        return std::get<1>(*it);
    }


    template<typename Q>
    TimeProbe<Q>::TimeProbe(Q& queue, Concurrency::Latch& barrier) :
        queue_(queue),
        barrier_(barrier){
    
        initialise();
    }


    template<typename Q>
    void TimeProbe<Q>::initialise() {

        t_initialTimePoint_ = system_clock::now();
        c_initialTimePoint_ = std::clock();
    }

    template<typename Q>
    void TimeProbe<Q>::logCurrentTime(double frameTime) {

        system_clock::time_point t_newTimePoint(system_clock::now());
        std::clock_t c_newTimePoint(std::clock());

        //using system clock
        double t_duration(duration_cast<std::chrono::nanoseconds>(t_newTimePoint - t_initialTimePoint_).count());
        t_frameProcessingTimes_.setTime(frameTime, t_duration*1e-9);

        //using cpu clock
        double c_duration(static_cast<double>(c_newTimePoint - c_initialTimePoint_) / CLOCKS_PER_SEC);
        c_frameProcessingTimes_.setTime(frameTime, c_duration);

    }

    template<typename Q>
    TimeData<double> TimeProbe<Q>::getWallClockTimes() const {
    
        return t_frameProcessingTimes_;
    }

    template<typename Q>
    TimeData<double> TimeProbe<Q>::getCpuClockTimes() const{
        
        return c_frameProcessingTimes_;
    }



    template<typename Q>
    void TimeProbe<Q>::operator()() {

        queue_.subscribe();
        barrier_.wait();
        initialise(); //reset the initial time point
        bool runCondition(true);
        while (runCondition) {
            auto frame(queue_.pop());
            if (!EndOfData::isEod(frame))
                logCurrentTime(frame.time);
            else
                runCondition = false;
        }

    }


    template<typename Qin, typename Qout>
    TimeDifference<Qin, Qout>::TimeDifference(
        Qin& queueIn,
        Qout& queueOut,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecutions) :
        queueIn_(queueIn),
        queueOut_(queueOut),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecutions_(doneWithExecutions)
        {
    }


    template<typename Qin, typename Qout>
    void TimeDifference<Qin, Qout>::operator()() {

        Concurrency::Latch internalLatch(3);

        TimeProbe<Qin> pIn(queueIn_, internalLatch);
        TimeProbe<Qout>  pOut(queueOut_, internalLatch);
        std::thread thrIn(std::ref(pIn));
        std::thread thrOut(std::ref(pOut));
        internalLatch.wait();
        doneWithSubscriptions_.wait();
        thrIn.join(); thrOut.join();
        doneWithExecutions_.wait();
     
        wallClockDifference_ = pOut.getWallClockTimes() - pIn.getWallClockTimes();
        cpuClockDifference_ = pOut.getCpuClockTimes() - pIn.getCpuClockTimes();
     }


    template<typename Qin, typename Qout>
    TimeData<double> TimeDifference<Qin, Qout>::getWallClockDifference() const {

        return wallClockDifference_;
    }

    template<typename Qin, typename Qout>
    TimeData<double> TimeDifference<Qin, Qout>::getCpuClockDifference() const {

        return cpuClockDifference_;
    }

}
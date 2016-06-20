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

#ifndef rtosim_IKoutputs_h
#define rtosim_IKoutputs_h

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace rtosim {

    template <typename T>
    class IKoutputs
    {
    public:
        IKoutputs() = default;
        IKoutputs(const IKoutputs&) = delete;            // disable copying
        IKoutputs& operator=(const IKoutputs&) = delete; // disable assignment
        T pop(double currentTime);
        T pop();
        void pop(T& item);
        void push(const T& item);

    private:

        struct OrderByTime
        {
            bool operator() (T const &a, T const &b) { return a.time > b.time; }
        };

        std::priority_queue<T, std::vector<T>, OrderByTime > queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}

#include "IKoutputs.cpp"
#endif

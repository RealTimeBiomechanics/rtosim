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

#ifndef rtosim_TimeSequence
#define rtosim_TimeSequence

#include "rtosim/concurrency/Queue.h"
#include <string>
#include <thread>
#include <queue>

namespace rtosim{

    class TimeSequence
    {
    public:
        TimeSequence() = default;
        TimeSequence(const TimeSequence&) = delete;            // disable copying
        TimeSequence& operator=(const TimeSequence&) = delete; // disable assignment
        double pop();
        void push(const double& item);

    private:
        std::queue<double> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}

#endif
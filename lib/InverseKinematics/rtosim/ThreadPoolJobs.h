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

#ifndef rtosim_ThreadPoolJobs_h
#define rtosim_ThreadPoolJobs_h

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace rtosim {

    template <typename T>
    class ThreadPoolJobs
    {
    public:
        ThreadPoolJobs() = default;
        ThreadPoolJobs(const ThreadPoolJobs&) = delete;            // disable copying
        ThreadPoolJobs& operator=(const ThreadPoolJobs&) = delete; // disable assignment
        T pop();
        size_t size();
        void pop(T& item);
        T front();
        void front(T& item);
        void push(const T& item);

    private:
        std::queue<T> queue_;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}

#include "ThreadPoolJobs.cpp"
#endif

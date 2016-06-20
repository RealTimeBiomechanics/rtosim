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

#ifndef rtosim_Latch_h
#define rtosim_Latch_h

#include <thread>
#include <mutex>
#include <condition_variable>

namespace rtosim{
    namespace Concurrency{
        class Latch {
        public:
            Latch();
            Latch(int count);
            void setCount(int count);
            void wait();
            Latch(const Latch&) = delete;
            Latch& operator=(const Latch&) = delete;
        private:
            int count_;
            std::condition_variable condition_;
            std::mutex mutex_;
        };
    }
}

#endif

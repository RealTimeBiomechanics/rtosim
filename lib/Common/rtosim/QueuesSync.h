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

#ifndef rtosim_QueuesSync_h
#define rtosim_QueuesSync_h

#include "rtosim/EndOfData.h"
#include <tuple>
#include <limits>
#include <thread>

namespace rtosim {
    namespace QueuesSync {

        template<typename T>
        typename T::type syncToTime(double t, T& queue) {

            typename T::type frame;
            do {
                frame = queue.pop();
            } while (frame.time < t && !EndOfData::isEod(frame));
            return frame;
        }

        template<typename A1, typename... Args>
        auto sync(A1& queue1, Args&... queues) -> decltype(std::make_tuple(queue1.pop(), queues.pop()...)) {

            auto frame1(queue1.pop());
            return std::make_tuple(frame1, syncToTime(frame1.time, queues)...);
        }

        template<typename... Args>
        void launchThreads(Args&... args) {

            auto lst = { std::make_unique<std::thread>(std::ref(args))... };
            for (auto& l : lst)
                if (l->joinable())
                    l->join();
        }
    }
}
#endif

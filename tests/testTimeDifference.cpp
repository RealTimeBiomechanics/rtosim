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

#include "rtosim/rtosim.h"
#include "rtosim/TimeDifference.h"
#include <thread>
#include <chrono>
#include <iostream>
using namespace rtosim;


struct Delay {

    template<typename T>
    T operator()(const T& v) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << v.time << std::endl;
        return v;
    }
};


int main() {

    rtosim::Concurrency::Latch l1(3), l2(3);
    ScalarVectorQueue q1, q2;
    Delay introduceDelay;
    QueueAdapter<ScalarVectorQueue, ScalarVectorQueue, Delay> delayer(q1, q2, l1, l2, introduceDelay);
    TimeDifference<ScalarVectorQueue, ScalarVectorQueue> timeDifference(q1, q2, l1, l2);

    auto dataPush([&](){  
        l1.wait();
        for (int i(0); i < 20; ++i) {
            q1.push(ScalarVectorFrame{ i, { 0 } });
        }
        q1.push(EndOfData::get<ScalarVectorFrame>());
        l2.wait();
    });

    QueuesSync::launchThreads(delayer, timeDifference, dataPush);
    auto wallClock(timeDifference.getWallClockDifference()); 
    std::cout << wallClock;
    wallClock.print("timeDifference.txt");
    return 0;
    
}
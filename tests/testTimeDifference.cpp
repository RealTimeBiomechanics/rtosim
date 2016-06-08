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

    rtosim::Concurrency::Latch l1(2), l2(2);
    ScalarVectorQueue q1, q2;
    Delay introduceDelay;
    QueueAdapter<ScalarVectorQueue, ScalarVectorQueue, Delay> delayer(q1, q2, l1, l2, introduceDelay);
    TimeDifference<ScalarVectorQueue, ScalarVectorQueue> timeDifference(q1, q2);

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
    return 0;
    
}
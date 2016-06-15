#ifndef rtosim_GeneralisedCoordinatesFromX_h
#define rtosim_GeneralisedCoordinatesFromX_h

#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include <string>
#include <vector>

namespace rtosim {
    class GeneralisedCoordinatesFromX {

    public:
        GeneralisedCoordinatesFromX(
            GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution);
        GeneralisedCoordinatesFromX(const GeneralisedCoordinatesFromX&) = delete;
        virtual ~GeneralisedCoordinatesFromX() {};
        virtual void operator()() = 0;
        void updateGeneralisedCoordinates(const GeneralisedCoordinatesData& currentExternalForceData, double currentTime);
        void doneWithSubscriptions();
        void doneWithExecution();
        void sendEndOfData();
    private:
        GeneralisedCoordinatesQueue& outputGeneralisedCoordinatesQueue_;
        Concurrency::Latch& doneWithSubscriptions_;
        Concurrency::Latch& doneWithExecution_;
    };
}
#endif

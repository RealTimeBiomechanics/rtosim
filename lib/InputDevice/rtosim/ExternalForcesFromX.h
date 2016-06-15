#ifndef rtosim_ExternalForcesFromX_h
#define rtosim_ExternalForcesFromX_h

#include <string>
#include <vector>
#include "rtosim/ExternalForceData.h"
#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"

namespace rtosim {
    class ExternalForcesFromX
    {
    public:

        ExternalForcesFromX(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution);
        virtual ~ExternalForcesFromX() {};
        virtual void operator()() = 0;
        void updateExternalForces(const MultipleExternalForcesData& currentExternalForceData, double currentTime);
        void sendEndOfData();
        void doneWithSubscriptions();
        void doneWithExecution();
    private:
        MultipleExternalForcesQueue& outputMultipleExternalForcesQueue_;
        Concurrency::Latch& doneWithSubscriptions_;
        Concurrency::Latch& doneWithExecution_;
    };
}
#endif

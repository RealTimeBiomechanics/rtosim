//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

#ifndef rtosim_IKSequencer_h
#define rtosim_IKSequencer_h

#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include "rtosim/TimeSequence.h"
#include "rtosim/IKoutputs.h"

#include <string>
#include <thread>

namespace rtosim{
    class IKSequencer{

    public:
        IKSequencer() = delete;
        IKSequencer(
            IKoutputs<rtosim::GeneralisedCoordinatesFrame>& inputGeneralisedCoordinateFrameFromIK,
            TimeSequence& inputTimeSequence,
            rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            unsigned numberOfIKSolver);
        virtual ~IKSequencer();
        void operator()();
    private:
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& inputGeneralisedCoordinateFrameFromIK_;
        TimeSequence& inputTimeSequence_;
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        unsigned numberOfIKSolver_;
    };
}

#endif

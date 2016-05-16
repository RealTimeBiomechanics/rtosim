//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

//#include "vosl/SharedData.h"
#include "rtosim/IKSequencer.h"
#include "rtosim/EndOfData.h"

namespace rtosim{

    IKSequencer::IKSequencer(
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& inputGeneralisedCoordinateFrameFromIK,
        TimeSequence& inputTimeSequence,
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        unsigned numberOfIKSolver) :
        inputGeneralisedCoordinateFrameFromIK_(inputGeneralisedCoordinateFrameFromIK),
        inputTimeSequence_(inputTimeSequence),
        outputGeneralisedCoordinateQueue_(outputGeneralisedCoordinateQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        numberOfIKSolver_(numberOfIKSolver) {}

    void IKSequencer::operator()(){

        bool localRunCondition(true);
        doneWithSubscriptions_.wait();

        while (localRunCondition) {
            double nextTime = inputTimeSequence_.pop();
            if (nextTime != rtosim::EndOfData::get<GeneralisedCoordinatesFrame>().time) {
                GeneralisedCoordinatesFrame ikFrame = inputGeneralisedCoordinateFrameFromIK_.pop(nextTime);
                outputGeneralisedCoordinateQueue_.push(ikFrame);
            }
            else {
                outputGeneralisedCoordinateQueue_.push(rtosim::EndOfData::get<GeneralisedCoordinatesFrame>());
                localRunCondition = false;
            }
        }
        doneWithExecution_.wait();
    }

    IKSequencer::~IKSequencer()
    {
    }
}
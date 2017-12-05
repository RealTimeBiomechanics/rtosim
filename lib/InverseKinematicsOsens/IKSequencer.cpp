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

#include "rtosim/IKSequencer.h"
#include "rtosim/EndOfData.h"

namespace rtosim{

    IKSequencer::IKSequencer(
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& inputGeneralisedCoordinateFrameFromIK,
        TimeSequence& inputTimeSequence,
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
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
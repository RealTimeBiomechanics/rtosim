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

#ifndef rtosim_IKSequencer_h
#define rtosim_IKSequencer_h

#include "rtb/concurrency/Queue.h"
#include "rtb/concurrency/Latch.h"
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
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            unsigned numberOfIKSolver);
        virtual ~IKSequencer();
        void operator()();
    private:
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& inputGeneralisedCoordinateFrameFromIK_;
        TimeSequence& inputTimeSequence_;
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        unsigned numberOfIKSolver_;
    };
}

#endif

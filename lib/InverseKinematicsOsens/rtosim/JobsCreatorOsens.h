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

#ifndef rtosim_JobsCreatorOsens_h
#define rtosim_JobsCreatorOsens_h

#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/IKSequencer.h"
#include "rtb/Concurrency/Concurrency.h"

#include <string>
#include <thread>

namespace rtosim {
  template<typename Q>
    class JobsCreatorOsens{

    public:
	using FrameType = typename Q::type;
        JobsCreatorOsens() = delete;
        JobsCreatorOsens(JobsCreatorOsens&) = delete;
        JobsCreatorOsens(
            Q& inputMarkerSetQueue,
            ThreadPoolJobs<FrameType>& outputJobsQueue,
            TimeSequence& outputTImeSequenceQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            unsigned noIkThreads);
        virtual ~JobsCreatorOsens();
        void operator()();
    private:
        Q& inputMarkerQueue_;
        ThreadPoolJobs<FrameType>& outputJobsQueue_;
        TimeSequence& outputTimeSequenceQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        unsigned noIkThreads_;
    };
}
#include "JobsCreatorOsens.cpp"
#endif

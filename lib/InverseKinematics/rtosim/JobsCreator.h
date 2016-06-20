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

#ifndef rtosim_JobsCreator_h
#define rtosim_JobsCreator_h

#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/IKSequencer.h"
#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/MarkerSetQueue.h"

#include <string>
#include <thread>

namespace rtosim {
    class JobsCreator{

    public:
        JobsCreator() = delete;
        JobsCreator(JobsCreator&) = delete;
        JobsCreator(
            MarkerSetQueue& inputMarkerSetQueue,
            ThreadPoolJobs<MarkerSetFrame>& outputJobsQueue,
            TimeSequence& outputTImeSequenceQueue,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            unsigned noIkThreads);
        virtual ~JobsCreator();
        void operator()();
    private:
        MarkerSetQueue& inputMarkerQueue_;
        ThreadPoolJobs<MarkerSetFrame>& outputJobsQueue_;
        TimeSequence& outputTimeSequenceQueue_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        unsigned noIkThreads_;
    };
}

#endif

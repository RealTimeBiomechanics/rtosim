//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

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

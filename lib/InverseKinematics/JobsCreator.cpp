//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

//#include "vosl/SharedData.h"
#include "rtosim/JobsCreator.h"
#include "rtosim/EndOfData.h"

#include <chrono>
#include <thread>
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

namespace rtosim {
    JobsCreator::JobsCreator(
        MarkerSetQueue& inputMarkerQueue,
        ThreadPoolJobs<MarkerSetFrame>& outputJobsQueue,
        TimeSequence& outputTimeSequenceQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        unsigned noIkThreads) :
        inputMarkerQueue_(inputMarkerQueue),
        outputJobsQueue_(outputJobsQueue),
        outputTimeSequenceQueue_(outputTimeSequenceQueue),
        doneWithSubscriptions_(doneWithSubscriptions),
        doneWithExecution_(doneWithExecution),
        noIkThreads_(noIkThreads) {}

    void JobsCreator::operator()(){

        bool localRunCondition(true);
        inputMarkerQueue_.subscribe();
        doneWithSubscriptions_.wait();
        //how to deal with runconditions?
        while (localRunCondition)  {
            MarkerSetFrame currentFrame = inputMarkerQueue_.pop();
            if (rtosim::EndOfData::isEod(currentFrame)) {
                localRunCondition = false;
                for (unsigned i = 0; i < noIkThreads_ - 1; ++i) {
                    outputJobsQueue_.push(currentFrame);
                }
            }

            outputJobsQueue_.push(currentFrame);
            outputTimeSequenceQueue_.push(currentFrame.time);
        }
        doneWithExecution_.wait();
    }

    JobsCreator::~JobsCreator()
    {
#ifdef RTOSIM_DEBUG
        //    cout << "Closing JobsCreator... " << endl << std::flush;
#endif
    }
}
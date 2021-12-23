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
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
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
        while (localRunCondition)  {
            MarkerSetFrame currentFrame = inputMarkerQueue_.pop().value();
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
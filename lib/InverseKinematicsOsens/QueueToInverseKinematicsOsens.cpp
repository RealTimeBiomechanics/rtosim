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

#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;
#include <thread>
using std::thread;
using std::ref;
#include "rtosim/QueueToInverseKinematicsOsens.h"
#include "rtosim/EndOfData.h"

namespace rtosim {

    QueueToInverseKinematicsOsens::QueueToInverseKinematicsOsens(
	OrientationSetQueue& inputOrientationSetQueue,
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        unsigned nThreads,
        double solverAccuracy,
        double contraintWeight) :
        inputOrientationSetQueue_(inputOrientationSetQueue),
        outputGeneralisedCoordinateQueue_(outputGeneralisedCoordinateQueue),
        doneWithExecution_(doneWithExecution),
        doneWithSubscriptions_(doneWithSubscriptions),
        osimModelFilename_(osimModelFilename),
        nThreads_(nThreads),
        useIkTaskSet_(false),
        solverAccuracy_(solverAccuracy),
        constraintWeight_(contraintWeight){

	  std::cout << "Constructor!!\n";
        if (nThreads_ < 1) nThreads_ = 1;
    }

    QueueToInverseKinematicsOsens::QueueToInverseKinematicsOsens(
        OrientationSetQueue& inputOrientationSetQueue,
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        unsigned nThreads,
        const std::string& ikTaskSetFilename,
        double solverAccuracy,
        double contraintWeight) :
        QueueToInverseKinematicsOsens(
        inputOrientationSetQueue,
        outputGeneralisedCoordinateQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        nThreads,
        solverAccuracy,
        contraintWeight) {

        ikTaskSetFilename_ = ikTaskSetFilename;
        useIkTaskSet_ = true;
    }

    void QueueToInverseKinematicsOsens::operator()() {

        ThreadPoolJobs<OrientationSetFrame> threadPoolJobQueue;
        TimeSequence timeSequenceQueue;
        IKoutputs<rtosim::GeneralisedCoordinatesFrame> ikOutputQueue;
        rtosim::GeneralisedCoordinatesQueue orderedGeneralisedCoordinateQueue;
        rtosim::Concurrency::Latch internalDoneWithSubscriptions(3 + nThreads_), internalDoneWithexecution(2 + nThreads_);
        JobsCreatorOsens<OrientationSetQueue> jobCreator(
	  inputOrientationSetQueue_, 
	  threadPoolJobQueue, 
	  timeSequenceQueue, 
	  internalDoneWithSubscriptions, 
	  internalDoneWithexecution, 
	  nThreads_);
        vector<unique_ptr<IKSolverParallelOsens>> ikSolvers;

        for (unsigned i(0); i < nThreads_; ++i)
            ikSolvers.emplace_back(new IKSolverParallelOsens(
                threadPoolJobQueue, 
                ikOutputQueue, 
                internalDoneWithSubscriptions, 
                internalDoneWithexecution, 
                osimModelFilename_, 
                solverAccuracy_, 
                constraintWeight_));

	    if (useIkTaskSet_) {
            for (auto& it : ikSolvers)
                it->setInverseKinematicsTaskSet(ikTaskSetFilename_);
        }
        IKSequencer ikSequencer(ikOutputQueue, timeSequenceQueue, outputGeneralisedCoordinateQueue_, internalDoneWithSubscriptions, internalDoneWithexecution, nThreads_);
        thread jobCreatorThr(ref(jobCreator));
        thread ikSequencerThr(ref(ikSequencer));
        vector<thread> ikSolversThrs;
        for (auto& it : ikSolvers)
            ikSolversThrs.emplace_back(ref(*it));
        internalDoneWithSubscriptions.wait();
        doneWithSubscriptions_.wait();
        std::cout << "starting IK" << std::endl;
        jobCreatorThr.join();
        ikSequencerThr.join();
        for (auto& it : ikSolversThrs)
            it.join();

        outputGeneralisedCoordinateQueue_.push(rtosim::EndOfData::get<rtosim::GeneralisedCoordinatesFrame>());
        //      std::cout << "IKFromQueue waiting\n";
        doneWithExecution_.wait();

        for (auto& it : ikSolvers)
            stopWatch_.push_back(it->getProcessingTimes());
    }
}
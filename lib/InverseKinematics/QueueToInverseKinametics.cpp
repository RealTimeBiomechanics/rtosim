#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;
#include <thread>
using std::thread;
using std::ref;
#include "rtosim/QueueToInverseKinametics.h"
#include "rtosim/EndOfData.h"

namespace rtosim {

    QueueToInverseKinametics::QueueToInverseKinametics(MarkerSetQueue& inputMarkerSetQueue,
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        unsigned nThreads,
        double solverAccuracy) :
        inputMarkerSetQueue_(inputMarkerSetQueue),
        outputGeneralisedCoordinateQueue_(outputGeneralisedCoordinateQueue),
        doneWithExecution_(doneWithExecution),
        doneWithSubscriptions_(doneWithSubscriptions),
        osimModelFilename_(osimModelFilename),
        nThreads_(nThreads),
        useIkTaskSet_(false),
        solverAccuracy_(solverAccuracy){

        if (nThreads_ < 1) nThreads_ = 1;
    }

    QueueToInverseKinametics::QueueToInverseKinametics(
        MarkerSetQueue& inputMarkerSetQueue,
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
        rtosim::Concurrency::Latch& doneWithSubscriptions,
        rtosim::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        unsigned nThreads,
        const std::string& ikTaskSetFilename,
        double solverAccuracy) :
        QueueToInverseKinametics(
        inputMarkerSetQueue,
        outputGeneralisedCoordinateQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        nThreads,
        solverAccuracy) {

        ikTaskSetFilename_ = ikTaskSetFilename;
        useIkTaskSet_ = true;
    }

    void QueueToInverseKinametics::operator()() {

        ThreadPoolJobs<MarkerSetFrame> threadPoolJobQueue;
        TimeSequence timeSequenceQueue;
        IKoutputs<rtosim::GeneralisedCoordinatesFrame> ikOutputQueue;
        rtosim::GeneralisedCoordinatesQueue orderedGeneralisedCoordinateQueue;
        rtosim::Concurrency::Latch internalDoneWithSubscriptions(3 + nThreads_), internalDoneWithexecution(2 + nThreads_);

        JobsCreator jobCreator(inputMarkerSetQueue_, threadPoolJobQueue, timeSequenceQueue, internalDoneWithSubscriptions, internalDoneWithexecution, nThreads_);
        vector<unique_ptr<IKSolverParallel>> ikSolvers;
        for (unsigned i(0); i < nThreads_; ++i)
            ikSolvers.emplace_back(new IKSolverParallel(threadPoolJobQueue, ikOutputQueue, internalDoneWithSubscriptions, internalDoneWithexecution, osimModelFilename_, solverAccuracy_));

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
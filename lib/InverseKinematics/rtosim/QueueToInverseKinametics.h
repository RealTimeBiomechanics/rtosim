//CP

#ifndef rtosim_QueueToInverseKinametics_h
#define rtosim_QueueToInverseKinametics_h

#include <string>
#include <memory>
#include <Simbody.h>

#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/JobsCreator.h"
#include "rtosim/IKSequencer.h"
#include "rtosim/IKSolverParallel.h"
#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/MarkerSetQueue.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include "rtosim/StopWatch.h"

namespace rtosim {

    class QueueToInverseKinametics {
    public:
        QueueToInverseKinametics(
            MarkerSetQueue& inputMarkerSetQueue,
            rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            unsigned nThreads,
            double solverAccuracy = 1e-9,
            double contraintWeight = SimTK::Infinity);

        QueueToInverseKinametics(
            MarkerSetQueue& inputMarkerSetQueue,
            rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            unsigned nThreads,
            const std::string& ikTaskSetFilename,
            double solverAccuracy = 1e-9,
            double contraintWeight = SimTK::Infinity);
        void operator()();

        std::vector < StopWatch > getProcessingTimes() { return stopWatch_; }
    private:
        MarkerSetQueue& inputMarkerSetQueue_;
        rtosim::GeneralisedCoordinatesQueue& outputGeneralisedCoordinateQueue_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        unsigned nThreads_;
        std::string osimModelFilename_;
        std::string ikTaskSetFilename_;
        bool useIkTaskSet_;
        double solverAccuracy_;
        double contraintWeight_;
        std::vector < StopWatch > stopWatch_;
    };
}

#endif

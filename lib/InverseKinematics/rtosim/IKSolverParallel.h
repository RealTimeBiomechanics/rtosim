//__________________________________________________________________________
// Author(s): Monica Reggiani - June 2014
// email:  monica.reggiani@gmail.com
//
// DO NOT REDISTRIBUTE WITHOUT PERMISSION
//__________________________________________________________________________
//

#ifndef rtosim_IKSolverParallel_h
#define rtosim_IKSolverParallel_h

#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include "rtosim/queue/MarkerSetQueue.h"
#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/IKoutputs.h"
#include "rtosim/StopWatch.h"

#include <string>
#include <map>
#include <OpenSim/OpenSim.h>

namespace rtosim{

    class IKSolverParallel{

    public:
        IKSolverParallel() = delete;
        IKSolverParallel(IKSolverParallel&) = delete;
        IKSolverParallel(
            ThreadPoolJobs<MarkerSetFrame>& inputThreadPoolJobs,
            IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            double solverAccuracy = 1e-8
            );
        void setInverseKinematicsTaskSet(const std::string& ikTaskSetFilename);
        virtual ~IKSolverParallel();
        void operator()();
        StopWatch getProcessingTimes() const { return stopWatch_; }

    private:
        void pushState(const SimTK::State& s);
        bool isWithinRom(const SimTK::State& s) const;
        ThreadPoolJobs<MarkerSetFrame>& inputThreadPoolJobs_;
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        const std::string& osimModelFilename_;
        OpenSim::Model model_;
        std::vector<std::string> markerNames_, coordinateNames_;
        unsigned nMarkers_, nCoordinates_;
        std::map<std::string, double> markerWeights_;
        rtosim::GeneralisedCoordinatesData defaultPose_;
        double sovlerAccuracy_;
        StopWatch stopWatch_;
    };
}

#endif

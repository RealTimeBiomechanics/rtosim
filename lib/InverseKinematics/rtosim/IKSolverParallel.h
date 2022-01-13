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

#ifndef rtosim_IKSolverParallel_h
#define rtosim_IKSolverParallel_h

#include "rtb/concurrency/Queue.h"
#include "rtb/concurrency/Latch.h"
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
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            double solverAccuracy = 1e-8,
            double contraintWeight = SimTK::Infinity
            );
        void setInverseKinematicsTaskSet(const std::string& ikTaskSetFilename);
        void setInverseKinematicsTaskSet(const OpenSim::IKTaskSet& ikTaskSet);
        virtual ~IKSolverParallel();
        void operator()();
        StopWatch getProcessingTimes() const { return stopWatch_; }

    private:
        void pushState(const SimTK::State& s);
        bool isWithinRom(const SimTK::State& s) const;
        ThreadPoolJobs<MarkerSetFrame>& inputThreadPoolJobs_;
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        const std::string& osimModelFilename_;
        OpenSim::Model model_;
        std::vector<std::string> markerNames_, coordinateNames_;
        unsigned nMarkers_, nCoordinates_;
        std::map<std::string, double> markerWeights_;
        rtosim::GeneralisedCoordinatesData defaultPose_;
        double sovlerAccuracy_;
        double contraintWeight_;
        StopWatch stopWatch_;
    };
}

#endif

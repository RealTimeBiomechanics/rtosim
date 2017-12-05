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

#ifndef rtosim_IKSolverParallelOsens_h
#define rtosim_IKSolverParallelOsens_h

#include "rtb/Concurrency/Concurrency.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include "rtosim/queue/OrientationSetQueue.h"
#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/IKoutputs.h"
#include "rtosim/StopWatch.h"

#include <string>
#include <map>
#include <OpenSim/OpenSim.h>

namespace rtosim{

    class IKSolverParallelOsens{

    public:
        IKSolverParallelOsens() = delete;
        IKSolverParallelOsens(IKSolverParallelOsens&) = delete;
        IKSolverParallelOsens(
            ThreadPoolJobs<OrientationSetFrame>& inputThreadPoolJobs,
            IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            double solverAccuracy = 1e-8,
            double constraintWeight = SimTK::Infinity
            );
        void setInverseKinematicsTaskSet(const std::string& ikTaskSetFilename);
        virtual ~IKSolverParallelOsens();
        void operator()();
        StopWatch getProcessingTimes() const { return stopWatch_; }

    private:
        void pushState(const SimTK::State& s);
        bool isWithinRom(const SimTK::State& s) const;
        ThreadPoolJobs<OrientationSetFrame>& inputThreadPoolJobs_;
        IKoutputs<rtosim::GeneralisedCoordinatesFrame>& outputGeneralisedCoordinatesQueue_;
        rtb::Concurrency::Latch& doneWithSubscriptions_;
        rtb::Concurrency::Latch& doneWithExecution_;
        const std::string& osimModelFilename_;
        OpenSim::Model model_;
        std::vector<std::string> names_, coordinateNames_;
        unsigned nOsens_, nCoordinates_;
        std::map<std::string, double> weights_;
        rtosim::GeneralisedCoordinatesData defaultPose_;
        double sovlerAccuracy_;
        double constraintWeight_;
        StopWatch stopWatch_;
    };
}

#endif

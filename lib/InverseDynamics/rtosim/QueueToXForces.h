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

#ifndef rtosim_QueuesToXForces_h
#define rtosim_QueuesToXForces_h
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <algorithm>
#include <OpenSim/OpenSim.h>

#include "rtb/concurrency/Queue.h"
#include "rtb/concurrency/Latch.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/queue/SingleExternalForceQueue.h"
#include "rtosim/ExternalForceProperties.h"
#include "rtosim/JointContactFromOpensim.h"

namespace rtosim {

    class QueueToXForces {
    public:

        QueueToXForces(
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& externalLoadsXmlFilename);

        QueueToXForces(
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::vector<ExternalForceProperties>& externalForceProperties);

        virtual ~QueueToXForces();
        virtual void operator()() = 0;
        void setState(double time, const std::vector<double>& q, const std::vector<double>& qd, const std::vector<double>& qdd);
        void calculateInverseDynamics();
        void getResidualMobilityForces(std::vector<double>& residualMobilityForces) const; //i.e. result of ID
        void doneWithSubscriptions();
        void doneWithExecution();
        void initialiseContactModel();

    private:
        static SimTK::Vector toSimTKVector(std::vector<double> v);
        static std::vector<double> fromSimTKVector(SimTK::Vector v);
        void distributeExternalForces();
        void updateCeinmsToOpenSimDofMapping();
        void updateCeinmsToOpenSimMuscleMapping();
        void initialiseExternalForces();
        MultipleExternalForcesQueue& inputExternalForcesQueue_;
        rtb::Concurrency::Latch splitForcesLatch_;
        rtb::Concurrency::Latch& externalDoneWithSubscriptionsLatch_;
        rtb::Concurrency::Latch& externalDoneWithExecutionLatch_;
        std::future<void> splitForcesThread_;
        std::vector<OpenSim::Force*> extForces_;
        std::vector<SingleExternalForceQueue*> singleExternalForceQueues_;
        std::unique_ptr<JointContactFromOpensim> jointContactModel_;
        //  Mapper  dofsMapping_, musclesMapping_;
        std::string osimModelFilename_;
        std::vector<ExternalForceProperties> externalForcesProperties_;
    };
}

#endif
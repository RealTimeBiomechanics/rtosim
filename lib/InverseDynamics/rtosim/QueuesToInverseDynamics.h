/* -------------------------------------------------------------------------- *
* CEINMS is a standalone toolbox for neuromusculoskeletal modelling and      *
* simulation. CEINMS can also be used as a plugin for OpenSim either         *
* through the OpenSim GUI or API. See https://simtk.org/home/ceinms and the  *
* NOTICE file for more information. CEINMS development was coordinated       *
* through Griffith University and supported by the Australian National       *
* Health and Medical Research Council (NHMRC), the US National Institutes of *
* Health (NIH), and the European Union Framework Programme 7 (EU FP7). Also  *
* see the PROJECTS file for more information about the funding projects.     *
*                                                                            *
* Copyright (c) 2010-2015 Griffith University and the Contributors           *
*                                                                            *
* CEINMS Contributors: C. Pizzolato, M. Reggiani, M. Sartori,                *
*                      E. Ceseracciu, and D.G. Lloyd                         *
*                                                                            *
* Author(s): C. Pizzolato                                                    *
*                                                                            *
* CEINMS is licensed under the Apache License, Version 2.0 (the "License").  *
* You may not use this file except in compliance with the License. You may   *
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.*
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
* -------------------------------------------------------------------------- */

#ifndef rtosim_QueuesToInverseDynamics_h
#define rtosim_QueuesToInverseDynamics_h

#include "rtosim/QueueToXForces.h"

#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/concurrency/Queue.h"
#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/queue/ExternalTorquesQueue.h"
#include "rtosim/ExternalForceProperties.h"

namespace rtosim{

    class QueuesToInverseDynamics : public QueueToXForces {

    public:
        QueuesToInverseDynamics(
            GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            ExternalTorquesQueue& outputExternalTorquesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& externalLoadsXmlFilename);

        QueuesToInverseDynamics(
            GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            ExternalTorquesQueue& outputExternalTorquesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::vector<ExternalForceProperties>& externalForceDataProperties);

        virtual void operator()() override;
        virtual ~QueuesToInverseDynamics() override {}

    private:
        void sendEndOfData();
        GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue_;
        ExternalTorquesQueue& outputExternalTorquesQueue_;
        bool onlyLogSubjectDofs_;
    };
}

#endif
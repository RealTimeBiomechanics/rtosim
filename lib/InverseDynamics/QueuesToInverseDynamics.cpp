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

#include "rtosim/QueuesToInverseDynamics.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/EndOfData.h"

namespace rtosim {

    QueuesToInverseDynamics::QueuesToInverseDynamics(
        GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        ExternalTorquesQueue& outputExternalTorquesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& externalLoadsXmlFilename) :
        QueueToXForces(inputExternalForcesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        externalLoadsXmlFilename),
        inputGeneralisedCoordinatesQueue_(inputGeneralisedCoordinatesQueue),
        outputExternalTorquesQueue_(outputExternalTorquesQueue),
        stopWatch_("time-id-processing")
    { }

    QueuesToInverseDynamics::QueuesToInverseDynamics(
        GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        ExternalTorquesQueue& outputExternalTorquesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::vector<ExternalForceProperties>& externalForceDataProperties) :
        QueueToXForces(inputExternalForcesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        externalForceDataProperties),
        inputGeneralisedCoordinatesQueue_(inputGeneralisedCoordinatesQueue),
        outputExternalTorquesQueue_(outputExternalTorquesQueue),
        stopWatch_("time-id-processing")
    {
    }

    void QueuesToInverseDynamics::operator()() {

        QueueToXForces::initialiseContactModel();
        inputGeneralisedCoordinatesQueue_.subscribe();
        QueueToXForces::doneWithSubscriptions();

        bool runCondition(true);
        
        while (runCondition) {

            GeneralisedCoordinatesFrame generalisedCoordinatesFrame(inputGeneralisedCoordinatesQueue_.pop());

            if (EndOfData::isEod(generalisedCoordinatesFrame))
                runCondition = false;
            if (runCondition) {
                stopWatch_.init();
                QueueToXForces::setState(
                    generalisedCoordinatesFrame.time,
                    generalisedCoordinatesFrame.data.getQ(),
                    generalisedCoordinatesFrame.data.getQd(),
                    generalisedCoordinatesFrame.data.getQdd()
                    );

                QueueToXForces::calculateInverseDynamics();
                std::vector<double> residualMobilityForces;
                QueueToXForces::getResidualMobilityForces(residualMobilityForces);
                ExternalTorquesFrame externalTorquesFrame{ generalisedCoordinatesFrame.time, residualMobilityForces };
                outputExternalTorquesQueue_.push(externalTorquesFrame);
                stopWatch_.log();
            }
           
        }
        sendEndOfData();
        QueueToXForces::doneWithExecution();
    }

    void QueuesToInverseDynamics::sendEndOfData() {

        outputExternalTorquesQueue_.push(EndOfData::get<ExternalTorquesFrame>());
    }
}
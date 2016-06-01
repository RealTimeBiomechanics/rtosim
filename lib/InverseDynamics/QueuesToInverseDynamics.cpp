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

#include "rtosim/QueuesToInverseDynamics.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/EndOfData.h"

namespace rtosim {

    QueuesToInverseDynamics::QueuesToInverseDynamics(
        GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        ExternalTorquesQueue& outputExternalTorquesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::string& externalLoadsXmlFilename) :
        QueueToXForces(inputExternalForcesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        externalLoadsXmlFilename),
        inputGeneralisedCoordinatesQueue_(inputGeneralisedCoordinatesQueue),
        outputExternalTorquesQueue_(outputExternalTorquesQueue),
        stopWatch_("inverse_dynamics")
    { }

    QueuesToInverseDynamics::QueuesToInverseDynamics(
        GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
        MultipleExternalForcesQueue& inputExternalForcesQueue,
        ExternalTorquesQueue& outputExternalTorquesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& osimModelFilename,
        const std::vector<ExternalForceProperties>& externalForceDataProperties) :
        QueueToXForces(inputExternalForcesQueue,
        doneWithSubscriptions,
        doneWithExecution,
        osimModelFilename,
        externalForceDataProperties),
        inputGeneralisedCoordinatesQueue_(inputGeneralisedCoordinatesQueue),
        outputExternalTorquesQueue_(outputExternalTorquesQueue),
        stopWatch_("inverse_dynamics")
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
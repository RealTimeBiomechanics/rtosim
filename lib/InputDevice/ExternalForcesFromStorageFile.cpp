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
* Author(s): E. Ceseracciu, M. Reggiani                                      *
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

#include "rtosim/ExternalForcesFromStorageFile.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/ArrayConverter.h"
#include "rtosim/FileSystem.h"

#include <OpenSim/OpenSim.h>
using OpenSim::Model;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>

namespace rtosim {

    ExternalForcesFromStorageFile::ExternalForcesFromStorageFile(
        MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& externalLoadsXmlFilename) :
        ExternalForcesFromX(outputMultipleExternalForcesQueue, doneWithSubscriptions, doneWithExecution),
        externalLoadProperties_(externalLoadsXmlFilename),
        externalForcesStorage_(externalLoadProperties_.getDatafile()){
    }

    ExternalForcesFromStorageFile::ExternalForcesFromStorageFile(
        MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
        Concurrency::Latch& doneWithSubscriptions,
        Concurrency::Latch& doneWithExecution,
        const std::string& externalLoadsXmlFilename,
        const std::string& grfFilename) :
        ExternalForcesFromX(outputMultipleExternalForcesQueue, doneWithSubscriptions, doneWithExecution),
        externalLoadProperties_(externalLoadsXmlFilename),
        externalForcesStorage_(grfFilename)  {
    }

    SimTK::Vec3 ExternalForcesFromStorageFile::getForce(const std::string& forceName, int timeIndex) const {

        string forceIdentifier = externalLoadProperties_.getForceIdentifier(forceName);
        OpenSim::Array<double> data;
        OpenSim::Array<int> indexes(externalForcesStorage_.getColumnIndicesForIdentifier(forceIdentifier));

        SimTK::Vec3 force(0.);
        for (int i(0); i < indexes.getSize(); ++i)
            externalForcesStorage_.getData(timeIndex, indexes.get(i) - 1, force[i]);
        return force;
    }

    SimTK::Vec3 ExternalForcesFromStorageFile::getTorque(const std::string& forceName, int timeIndex) const {

        string torqueIdentifier = externalLoadProperties_.getTorqueIdentifier(forceName);
        OpenSim::Array<double> data;
        OpenSim::Array<int> indexes(externalForcesStorage_.getColumnIndicesForIdentifier(torqueIdentifier));

        SimTK::Vec3 torque(0.);
        for (int i(0); i < indexes.getSize(); ++i)
            externalForcesStorage_.getData(timeIndex, indexes.get(i) - 1, torque[i]);
        return torque;
    }

    SimTK::Vec3 ExternalForcesFromStorageFile::getApplicationPoint(const std::string& forceName, int timeIndex) const {

        string pointIdentifier = externalLoadProperties_.getPointIdentifier(forceName);
        OpenSim::Array<double> data;
        OpenSim::Array<int> indexes(externalForcesStorage_.getColumnIndicesForIdentifier(pointIdentifier));

        SimTK::Vec3 point(0.);
        for (int i(0); i < indexes.getSize(); ++i)
            externalForcesStorage_.getData(timeIndex, indexes.get(i) - 1, point[i]);
        return point;
    }

    void ExternalForcesFromStorageFile::operator()() {

        OpenSim::Array<string> tempLabels(externalForcesStorage_.getColumnLabels());
        vector<string> labels;
        ArrayConverter::toStdVector(tempLabels, labels);

        unsigned sleepTimeMilliseconds(0);
        if (pushFrequency_ > 0)
            sleepTimeMilliseconds = static_cast<unsigned>(std::floor(1000. / (pushFrequency_)));

        ExternalForcesFromX::doneWithSubscriptions();
        std::vector<std::string> extForceNames(externalLoadProperties_.getExternalForcesNames());
        for (unsigned tIdx(0); tIdx < externalForcesStorage_.getSize(); ++tIdx) {

            if (pushFrequency_ > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMilliseconds));

            MultipleExternalForcesFrame externalForcesFrame;
            externalForcesStorage_.getTime(tIdx, externalForcesFrame.time);
            for (auto& name : extForceNames) {
                ExternalForceData currentExtForce;
                currentExtForce.setForceVector(getForce(name, tIdx));
                currentExtForce.setApplicationPoint(getApplicationPoint(name, tIdx));
                currentExtForce.setTorque(getTorque(name, tIdx));
                currentExtForce.setSourceName(name);
                externalForcesFrame.data.push_back(currentExtForce);
            }
            ExternalForcesFromX::updateExternalForces(externalForcesFrame.data, externalForcesFrame.time);
        }
        ExternalForcesFromX::sendEndOfData();
        ExternalForcesFromX::doneWithExecution();
    }
}
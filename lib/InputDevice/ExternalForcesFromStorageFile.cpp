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
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& externalLoadsXmlFilename,
        double fc) :
        ExternalForcesFromX(outputMultipleExternalForcesQueue, doneWithSubscriptions, doneWithExecution),
        externalLoadProperties_(externalLoadsXmlFilename),
        externalForcesStorage_(externalLoadProperties_.getDatafile()),
        speedFactor_(-1),
        framesToSkip_(0)
    {
        filter(fc);
    }

    ExternalForcesFromStorageFile::ExternalForcesFromStorageFile(
        MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
        rtb::Concurrency::Latch& doneWithSubscriptions,
        rtb::Concurrency::Latch& doneWithExecution,
        const std::string& externalLoadsXmlFilename,
        const std::string& grfFilename,
        double fc) :
        ExternalForcesFromX(outputMultipleExternalForcesQueue, doneWithSubscriptions, doneWithExecution),
        externalLoadProperties_(externalLoadsXmlFilename),
        externalForcesStorage_(grfFilename),
        speedFactor_(-1),
        framesToSkip_(0)
    {
        filter(fc);
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

    void ExternalForcesFromStorageFile::setOutputFrequency(double frequency) {

        speedFactor_ = frequency / sampleFrequency_;
    }

    void ExternalForcesFromStorageFile::setSpeedFactor(double speedFactor) {

        speedFactor_ = speedFactor;
    }

    void ExternalForcesFromStorageFile::setFramesToSkip(unsigned n) {

        framesToSkip_ = n;
    }

    std::chrono::milliseconds ExternalForcesFromStorageFile::getSleepTime() const {

        return std::chrono::milliseconds(static_cast<unsigned>(std::floor(1000. / (sampleFrequency_*speedFactor_))));
    }

    void ExternalForcesFromStorageFile::filter(double fc) {

        if (fc > 0) {
            double firstTime(externalForcesStorage_.getFirstTime());
            double lastTime(externalForcesStorage_.getLastTime());
            externalForcesStorage_.pad(externalForcesStorage_.getSize() / 2.);
            externalForcesStorage_.lowpassIIR(fc);
            externalForcesStorage_.crop(firstTime, lastTime);
        }
    }

    void ExternalForcesFromStorageFile::operator()() {

        const std::chrono::milliseconds sleepTimeMilliseconds(getSleepTime());
        unsigned skipped(0);

        OpenSim::Array<string> tempLabels(externalForcesStorage_.getColumnLabels());
        vector<string> labels;
        ArrayConverter::toStdVector(tempLabels, labels);

        ExternalForcesFromX::doneWithSubscriptions();
        std::vector<std::string> extForceNames(externalLoadProperties_.getExternalForcesNames());

        std::chrono::steady_clock::time_point timeOutTime = std::chrono::steady_clock::now();
        for (unsigned tIdx(0); tIdx < externalForcesStorage_.getSize(); ++tIdx) {

            if (speedFactor_>0) {
                timeOutTime += sleepTimeMilliseconds;
                std::this_thread::sleep_until(timeOutTime);
            }
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

            if (skipped == framesToSkip_) {
                ExternalForcesFromX::updateExternalForces(externalForcesFrame.data, externalForcesFrame.time);
                skipped = 0;
            }
            else
                ++skipped;
        }
        ExternalForcesFromX::sendEndOfData();
        ExternalForcesFromX::doneWithExecution();
    }
}

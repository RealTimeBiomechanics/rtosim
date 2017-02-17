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

#ifndef rtosim_ExternalForcesFromStorageFile_h
#define rtosim_ExternalForcesFromStorageFile_h

#include "rtosim/ExternalForcesFromX.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtb/concurrency/Concurrency.h"
#include "rtosim/ExternalLoadProperties.h"
#include <OpenSim/OpenSim.h>
#include <string>

namespace rtosim {

    class ExternalForcesFromStorageFile : public ExternalForcesFromX {
    public:
        //to parse the external forces easily, I need an opensim model.. otherwise I have to read manually from the storage
        ExternalForcesFromStorageFile(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& externalLoadsXmlFilename,
            double fc = -1);

        //using this constructor, grfFilename replace the grf in externalLoadsXml
        ExternalForcesFromStorageFile(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            rtb::Concurrency::Latch& doneWithSubscriptions,
            rtb::Concurrency::Latch& doneWithExecution,
            const std::string& externalLoadsXmlFilename,
            const std::string& grfFilename,
            double fc = -1);

        void setOutputFrequency(double frequency);
        void setSpeedFactor(double speedFactor);
        void setFramesToSkip(unsigned n);
        void operator()();

    private:
        void filter(double fc);
        SimTK::Vec3 getForce(const std::string& forceName, int timeIndex) const;
        SimTK::Vec3 getTorque(const std::string& forceName, int timeIndex) const;
        SimTK::Vec3 getApplicationPoint(const std::string& forceName, int timeIndex) const;
        std::chrono::milliseconds getSleepTime() const;

        ExternalLoadProperties externalLoadProperties_;
        OpenSim::Storage externalForcesStorage_;
        unsigned framesToSkip_;
        double speedFactor_;
        unsigned sampleFrequency_;
    };
}

#endif

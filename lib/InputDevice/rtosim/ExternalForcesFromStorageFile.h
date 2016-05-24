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

#ifndef rtosim_ExternalForcesFromStorageFile_h
#define rtosim_ExternalForcesFromStorageFile_h

#include "rtosim/ExternalForcesFromX.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/ExternalLoadProperties.h"
#include <OpenSim/OpenSim.h>
#include <string>

namespace rtosim {

    class ExternalForcesFromStorageFile : public ExternalForcesFromX {
    public:
        //to parse the external forces easily, I need an opensim model.. otherwise I have to read manually from the storage
        ExternalForcesFromStorageFile(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& externalLoadsXmlFilename);

        //using this constructor, grfFilename replace the grf in externalLoadsXml
        ExternalForcesFromStorageFile(
            MultipleExternalForcesQueue& outputMultipleExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& externalLoadsXmlFilename,
            const std::string& grfFilename);

        void operator()();
        void setPushFrequency(double f) { pushFrequency_ = f; }
    private:
        SimTK::Vec3 getForce(const std::string& forceName, int timeIndex) const;
        SimTK::Vec3 getTorque(const std::string& forceName, int timeIndex) const;
        SimTK::Vec3 getApplicationPoint(const std::string& forceName, int timeIndex) const;

        ExternalLoadProperties externalLoadProperties_;
        OpenSim::Storage externalForcesStorage_;
        double pushFrequency_;
        bool useCustomForceFile_;
    };
}

#endif
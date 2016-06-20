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

#ifndef rtosim_ExternalLoadProperties_h
#define rtosim_ExternalLoadProperties_h

#include "rtosim/ExternalForceProperties.h"
#include <Simbody.h>
#include <string>
#include <vector>

namespace rtosim {
    class ExternalLoadProperties {
    public:
        ExternalLoadProperties(const std::string& xmlFilename);
        std::size_t getExternalForcesSize() const;
        std::vector<ExternalForceProperties> getExternalForces() const;
        std::vector<std::string> getExternalForcesNames() const;
        std::string getForceIdentifier(const std::string& forceName) const;
        std::string getPointIdentifier(const std::string& forceName) const;
        std::string getTorqueIdentifier(const std::string& forceName) const;
        std::string getName() const;
        std::string getDatafile() const;
        std::string getKinematicsFile() const;
        double getCutoffFrequency() const;

    private:
        void parseXml(SimTK::Xml::Element elt);
        void parseExternalLoads(SimTK::Xml::Element elt);
        void parseObjects(SimTK::Xml::Element elt);
        std::string getCorrectFilePath(std::string file) const;
        std::vector < ExternalForceProperties > externalForcesProperties_;
        std::string xmlFilename_;
        std::string name_;
        std::string datafile_;
        std::string external_loads_model_kinematics_file_;
        double lowpass_cutoff_frequency_for_load_kinematics_;
    };
}

#endif
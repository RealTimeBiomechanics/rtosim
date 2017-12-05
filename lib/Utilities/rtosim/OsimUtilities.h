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

#ifndef rtosim_OsimUtilities_h
#define rtosim_OsimUtilities_h

#include <vector>
#include <OpenSim/OpenSim.h>
#include <OpenSim/Simulation/Model/ComponentSet.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>
#include <OpenSim/Simulation/Model/OrientationSensor.h>

namespace rtosim {
  
    void attachOsensToModel(OpenSim::Model& model);
    std::vector<std::string> getOsensNamesFromModel(const std::string& osimModelFilename); 
    std::vector<std::string> getMarkerNamesFromModel(const std::string& modelFilename);
    std::vector<std::string> getCoordinateNamesFromModel(const std::string& modelFilename);
    std::vector<std::string> getMarkerNamesOnBody(const OpenSim::Body& body);
    std::string getMostPosteriorMarker(const std::vector<std::string>& markerNames, OpenSim::Model& model);
}

#endif
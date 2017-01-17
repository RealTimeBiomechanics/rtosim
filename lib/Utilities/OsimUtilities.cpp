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

#include "rtosim/OsimUtilities.h"
#include "rtosim/ArrayConverter.h"
#include <OpenSim/Simulation/Model/OrientationSensor.h>
#include <OpenSim/Simulation/Model/OrientationSensorSet.h>

namespace rtosim {

  std::vector<std::string> getOsensNamesFromModel(const std::string& osimModelFilename) {

    OpenSim::Model model(osimModelFilename);
    const OpenSim::ComponentSet& componentSet(model.getMiscModelComponentSet());
    OpenSim::OrientationSensorSet orientationSensorSet;
	for(unsigned i(0); i < componentSet.getSize(); ++i) {
	  std::cout << "Component " << i << std::endl;
	  OpenSim::OrientationSensor* sensor(dynamic_cast<OpenSim::OrientationSensor*>(&componentSet.get(i)));
	  if(sensor != nullptr) {
	    orientationSensorSet.cloneAndAppend(*sensor);
	    std::cout << "Adding component\n";
	  }
	}
        OpenSim::Array<std::string> sensorsNamesArray;
	orientationSensorSet.getOSensorNames(sensorsNamesArray);
        std::vector<std::string> sensorsNamesFromModel;
	ArrayConverter::toStdVector(sensorsNamesArray, sensorsNamesFromModel);
    return sensorsNamesFromModel;
  
}
  
  void attachOsensToModel(OpenSim::Model& model) {
   
    const OpenSim::ComponentSet& componentSet(model.getMiscModelComponentSet());
    OpenSim::OrientationSensorSet orientationSensorSet;
    for(unsigned i(0); i < componentSet.getSize(); ++i) {
	  std::cout << "Component " << i << std::endl;
	  OpenSim::OrientationSensor* sensor(static_cast<OpenSim::OrientationSensor*>(&componentSet.get(i)));
	  if(sensor != nullptr) {
	    sensor->connectToModel(model);
	    std::string nameSens = sensor->getName();
	    std::string nameBody = sensor->getBody().getName();
	    std::cout << "Name: " << nameSens << " Body" << nameBody << std::endl;
	  }
    
    }    
  }
  
    std::vector<std::string> getMarkerNamesFromModel(const std::string& modelFilename) {

        OpenSim::Model model(modelFilename);
        OpenSim::Array<std::string> markerNamesOS;
        std::vector<std::string> markerNames;
        model.getMarkerSet().getNames(markerNamesOS);
        rtosim::ArrayConverter::toStdVector(markerNamesOS, markerNames);
        return markerNames;
    }

    std::vector<std::string> getCoordinateNamesFromModel(const std::string& modelFilename) {

        OpenSim::Model model(modelFilename);
        OpenSim::Array<std::string> coordNamesOS;
        std::vector<std::string> coordNames;
        model.getCoordinateSet().getNames(coordNamesOS);
        rtosim::ArrayConverter::toStdVector(coordNamesOS, coordNames);
        return coordNames;
    }

    std::vector<std::string> getMarkerNamesOnBody(const OpenSim::Body& body) {

        auto model(body.getModel());
        auto markerSet(model.getMarkerSet());
        std::vector<std::string> markers;
        for (unsigned i(0); i < static_cast<unsigned>(markerSet.getSize()); ++i) {
            if (markerSet.get(i).getBodyName() == body.getName())
                markers.emplace_back(markerSet.get(i).getName());
        }

        return markers;
    }

    std::string getMostPosteriorMarker(const std::vector<std::string>& markerNames, OpenSim::Model& model) {

        std::vector<double> x;
        for (auto& m : markerNames) {

            x.push_back(model.getMarkerSet().get(m).getOffset()[0]);
        }
        auto it(std::min_element(x.begin(), x.end()));
        return markerNames.at(std::distance(x.begin(), it));
    }
}
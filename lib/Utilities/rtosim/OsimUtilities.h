#ifndef rtosim_OsimUtilities_h
#define rtosim_OsimUtilities_h

#include <vector>
#include <OpenSim/OpenSim.h>

namespace rtosim {

    std::vector<std::string> getMarkerNamesFromModel(const std::string& modelFilename);

    std::vector<std::string> getCoordinateNamesFromModel(const std::string& modelFilename);

    std::vector<std::string> getMarkerNamesOnBody(const OpenSim::Body& body);

    std::string getMostPosteriorMarker(const std::vector<std::string>& markerNames, OpenSim::Model& model);
}

#endif
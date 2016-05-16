#ifndef rtosim_OsimUtilities_h
#define rtosim_OsimUtilities_h

#include <vector>
#include <OpenSim/OpenSim.h>

namespace rtosim {

    std::vector<std::string> getCoordinateNamesFromModel(const std::string& modelFilename);

    std::vector<std::string> getMarkerNamesFromModel(const std::string& modelFilename);
}

#endif
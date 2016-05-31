#include "rtosim/OsimUtilities.h"
#include "rtosim/ArrayConverter.h"

namespace rtosim {

    //you may want to use this to avoid getting the coordinate from a modified model
    /* std::vector<std::string> getCoordinateNamesFromModel(std::string modelFilename) {

         OpenSim::Model model(modelFilename);
         auto coordNames(getCoordinateNamesFromModel(model));
         return coordNames;
         }
         */

    /*
    std::vector<std::string> getMarkerNamesFromModel(std::string modelFilename) {

    OpenSim::Model model(modelFilename);
    auto markerNames(getMarkerNamesFromModel(model));
    return markerNames;
    }
    */
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
}
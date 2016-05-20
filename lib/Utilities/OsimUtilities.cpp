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
    std::vector<std::string> getCoordinateNamesFromModel(const std::string& modelFilename) {

        OpenSim::Model model(modelFilename);
        size_t nCoord(model.getNumCoordinates());
        OpenSim::Array<std::string> coordNamesOS;
        model.getCoordinateSet().getNames(coordNamesOS);
        std::vector<std::string> coordNames;
        rtosim::ArrayConverter::toStdVector(coordNamesOS, coordNames);
        return coordNames;
    }
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
}
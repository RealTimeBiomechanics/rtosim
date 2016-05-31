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
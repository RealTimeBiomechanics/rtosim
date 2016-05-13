#include "rtosim/MarkerData.h"

namespace rtosim {

    MarkerData::MarkerData() :
        coordinates_(SimTK::Vec3(SimTK::NaN)),
        isOccluded_(true) {}

    MarkerData::MarkerData(const SimTK::Vec3& coordinates) :
        coordinates_(coordinates), isOccluded_(false) {

        if (coordinates_ == SimTK::Vec3(0.))
            coordinates_ = SimTK::Vec3(SimTK::NaN);
    }

    MarkerData::MarkerData(double x, double y, double z) :
        coordinates_(SimTK::Vec3(x, y, z)), isOccluded_(false) {

        if (coordinates_ == SimTK::Vec3(0.))
            coordinates_ = SimTK::Vec3(SimTK::NaN);
    }

    SimTK::Vec3 MarkerData::getCoordinates() const {

        if (isOccluded_)
            return SimTK::Vec3(SimTK::NaN);
        else
            return coordinates_;
    }

    void MarkerData::setCoordinates(const SimTK::Vec3& coordinates) {

        coordinates_ = coordinates;
    }

    void MarkerData::setOccluded(bool isOccluded) {

        isOccluded_ = isOccluded;
    }

    bool MarkerData::getOccluded() const {

        return isOccluded_;
    }
}
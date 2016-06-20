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
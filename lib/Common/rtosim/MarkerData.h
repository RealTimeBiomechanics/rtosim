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

#ifndef rtosim_MarkerData_h
#define rtosim_MarkerData_h

#include <string>
#include <Simbody.h>

namespace rtosim{

    class MarkerData {

    public:
        MarkerData();
        MarkerData(const SimTK::Vec3& coordinates);
        MarkerData(double x, double y, double z);
        SimTK::Vec3 getCoordinates() const;
        void setCoordinates(const SimTK::Vec3& coordinates);
        void setOccluded(bool isOccluded);
        bool getOccluded() const;

    private:
        std::string label_;
        bool isOccluded_;
        SimTK::Vec3 coordinates_;
    };
}

#endif

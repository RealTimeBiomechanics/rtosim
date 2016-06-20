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

#include "rtosim/ExternalForceDataFilterStateSpace.h"

namespace rtosim {

    ExternalForceDataFilterStateSpace::ExternalForceDataFilterStateSpace(double fc, double forceThreshold) :
        fc_(fc), forceThreshold_(forceThreshold),
        dim_(3),
        grfFilter_(dim_),
        copFilter_(dim_),
        momentsFilter_(dim_),
        torquesFilter_(dim_){
    }

    ExternalForceData ExternalForceDataFilterStateSpace::filter(const ExternalForceData& data, double time) {

        //copy internal values.. in case we use the informations reguarding the body of application
        ExternalForceData filteredData(data);
        //I need to filter eveything, even if under the threshold
        //otherwise I could have problems in the filters state (i.e. frame skipping)
        filteredData.setForceVector(filter(data.getForce(), time, grfFilter_));
        filteredData.setApplicationPoint(filter(data.getApplicationPoint(), time, copFilter_));
        filteredData.setMoments(filter(data.getMoments(), time, momentsFilter_));
        filteredData.setTorque(filter(data.getTorque(), time, torquesFilter_));

        if (filteredData.getForce().norm() < forceThreshold_) {
            filteredData.setForceVector(SimTK::Vec3(0.));
            auto cop(filteredData.getApplicationPoint());
            //keep the original y coordinate of cop
            cop[0] = 0;
            cop[2] = 0;
            filteredData.setApplicationPoint(cop);
            filteredData.setTorque(SimTK::Vec3(0.));
        }

        return filteredData;
    }

    SimTK::Vec3 ExternalForceDataFilterStateSpace::filter(const SimTK::Vec3& vec, double time, std::vector< Filter::StateSpaceFilter<double> > & f) {

        SimTK::Vec3 ans;
        ans[0] = f[0].filter(vec[0], time, fc_);
        ans[1] = f[1].filter(vec[1], time, fc_);
        ans[2] = f[2].filter(vec[2], time, fc_);
        return ans;
    }
}
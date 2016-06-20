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

#include "rtosim/GeneralisedCoordinatesStateSpace.h"
#include "rtosim/SignalProcessingCommon.h"
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

namespace rtosim {

    GeneralisedCoordinatesStateSpace::GeneralisedCoordinatesStateSpace(double fc, size_t dimension) :
        dim_(dimension),
        fc_(fc)        {
        generalisedCoordinatesFilter_.resize(dimension);
    }

    rtosim::GeneralisedCoordinatesFrame GeneralisedCoordinatesStateSpace::filter(const rtosim::GeneralisedCoordinatesFrame& frame) {

        if (frame.data.getNCoordinates() != dim_) {
            cout << "GeneralisedCoordinatesDataFilter: number of coordinates differs from number of fileter.\n Exit.";
            //todo: add a graceful exit
            std::exit(EXIT_FAILURE);
        }
        auto filteredData(frame.data);

        std::vector<double> rawQ(frame.data.getQ());
        std::vector<double> q, qd, qdd;
        for (unsigned i(0); i < dim_; ++i) {
            generalisedCoordinatesFilter_.at(i).filter(rawQ.at(i), frame.time, fc_);
            q.push_back(generalisedCoordinatesFilter_.at(i).getFilteredValue());
            qd.push_back(generalisedCoordinatesFilter_.at(i).getFilteredFirstDerivative());
            qdd.push_back(generalisedCoordinatesFilter_.at(i).getFilteredSecondDerivative());
        }

        filteredData.setQ(q);
        filteredData.setQd(qd);
        filteredData.setQdd(qdd);

        return{ frame.time, filteredData };
    }
}
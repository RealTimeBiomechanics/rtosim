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

#include "rtosim/GeneralisedCoordinatesData.h"

namespace rtosim {

    GeneralisedCoordinatesData::GeneralisedCoordinatesData() :
        nCoordinates_(0)
    {}

    GeneralisedCoordinatesData::GeneralisedCoordinatesData(std::size_t nCoordinates) :
        nCoordinates_(nCoordinates),
        q_(nCoordinates, .0),
        qd_(nCoordinates, .0),
        qdd_(nCoordinates, .0)
    {}

    std::size_t GeneralisedCoordinatesData::getNCoordinates() const {

        return nCoordinates_;
    }

    void GeneralisedCoordinatesData::setQ(const std::vector<double>& q) {

        nCoordinates_ = q.size();
        q_ = q;
    }

    void GeneralisedCoordinatesData::setQd(const std::vector<double>& qd) {

        qd_ = qd;
    }

    void GeneralisedCoordinatesData::setQdd(const std::vector<double>& qdd) {

        qdd_ = qdd;
    }

    const std::vector<double>& GeneralisedCoordinatesData::getQ() const {

        return q_;
    }

    const std::vector<double>& GeneralisedCoordinatesData::getQd() const {

        return qd_;
    }

    const std::vector<double>& GeneralisedCoordinatesData::getQdd() const {

        return qdd_;
    }

    void GeneralisedCoordinatesData::getQ(std::vector<double>& q) const {

        q = q_;
    }

    void GeneralisedCoordinatesData::getQd(std::vector<double>& qd) const {

        qd = qd_;
    }

    void GeneralisedCoordinatesData::getQdd(std::vector<double>& qdd) const {

        qdd = qdd_;
    }
};
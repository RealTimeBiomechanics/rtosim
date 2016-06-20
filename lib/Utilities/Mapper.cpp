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

#include "rtosim/Mapper.h"
#include <vector>
#include <string>
#include <algorithm>

namespace rtosim {

    Mapper::Mapper() :
        inSize_(0),
        outSize_(0) {}

    Mapper::Mapper(const std::vector<std::string>& inNames, const std::vector<std::string>& outNames) {

        setNames(inNames, outNames);
    }

    void Mapper::setNames(const std::vector<std::string>& inNames, const std::vector<std::string>& outNames) {

        inToOutMapping_.clear();
        inSize_ = inNames.size();
        outSize_ = outNames.size();

        for (const auto& in : inNames) {
            auto it(std::find(outNames.cbegin(), outNames.cend(), in));
            if (it != outNames.cend())
                inToOutMapping_.push_back(std::distance(outNames.cbegin(), it));
            else inToOutMapping_.push_back(-1);
        }
    }
}
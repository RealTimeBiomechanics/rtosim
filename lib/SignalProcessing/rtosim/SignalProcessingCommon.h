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

#ifndef rtosim_SignalProcessingCommon_h
#define rtosim_SignalProcessingCommon_h

#include <Simbody.h>
#include <vector>

namespace rtosim {
    namespace SignalProcessingCommon {

        template<typename T, typename U>
        T filter(T& value, U& filt);

        template<typename U>
        SimTK::Vec3 filter(const SimTK::Vec3& vec, U& filt);

        //so I can filter both vector<Vec3> and vector<double>
        template<typename T, typename U>
        std::vector<T> filter(const std::vector<T>& v, std::vector<U>& filt);

        template<typename T>
        void resetFilters(T& filt);
    }
}

#include "SignalProcessingCommon.cpp"
#endif
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

namespace rtosim {
    namespace SignalProcessingCommon {

        template<typename T, typename U>
        T filter(T& value, U& filt) {

            return filt.filter(value);
        }

        template<typename U>
        SimTK::Vec3 filter(const SimTK::Vec3& vec, U& filt) {

            SimTK::Vec3 filteredVect(0.);
            filteredVect[0] = filt[0].filter(vec[0]);
            filteredVect[1] = filt[1].filter(vec[1]);
            filteredVect[2] = filt[2].filter(vec[2]);
            return filteredVect;
        }

        //so I can filter both vector<Vec3> and vector<double>
        template<typename T, typename U>
        std::vector<T> filter(const std::vector<T>& v, std::vector<U>& filt) {

            std::vector<T> filtered;
            filtered.reserve(v.size());
            auto vIt(v.cbegin());
            auto fIt(filt.begin());
            for (; (vIt != v.cend()) && (fIt != filt.end()); ++vIt, ++fIt) {
                auto idx(std::distance(v.cbegin(), vIt));
                filtered.emplace_back(filter(*vIt, *fIt));
            }
            return filtered;
        }

        template<typename T>
        void resetFilters(T& filt) {
            for (auto& f : filt)
                f.resetState();
        }
    }
}
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

#ifndef rtosim_QueueData_h
#define rtosim_QueueData_h

#include <ostream>
#include <vector>

namespace rtosim {

    template <typename T>
    struct QueueData;

    template<typename T>
    bool operator<(const QueueData<T>& lhs, const QueueData<T>& rhs) {
        return lhs.time < rhs.time;
    }

    template <typename T>
    struct QueueData {
        using type = T;
        double time;
        T data;

        template<typename Archive>
        inline void serialize(Archive& ar, const unsigned int version){

            ar & this->time;
            ar & this->data;
        }
    };
}
#endif

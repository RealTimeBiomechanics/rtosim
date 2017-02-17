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

#ifndef rtosim_MarkerSetQueue_h
#define rtosim_MarkerSetQueue_h

#include "rtosim/QueueData.h"
#include "rtb/concurrency/Queue.h"
#include "rtosim/MarkerData.h"
#include <vector>

namespace rtosim {

    using MarkerSetData = std::vector < MarkerData > ;
    using MarkerSetFrame = rtosim::QueueData < MarkerSetData > ;
    using MarkerSetQueue = rtb::Concurrency::Queue < MarkerSetFrame > ;
}

#endif
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

#include <OpenSim/OpenSim.h>
#include <string>
using namespace std;

#include "rtosim/ExternalForcesFromStorageFile.h"
#include "rtosim/Common.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/OutputDevice.h"

int main() {

    rtosim::MultipleExternalForcesQueue forcesQueue;
    rtosim::Concurrency::Latch subscriptionLatch, executionLatch;
    string filename = "C:/Data/RealTime/ElaboratedData/S04/20151120/inverseDynamics/q/walking_1ms_1/Setup/external_loads.xml";
    rtosim::ExternalForcesFromStorageFile reader(forcesQueue, subscriptionLatch, executionLatch, filename);
    //  rtosim::QueueToFileLogger<rtosim::MultipleExternalForcesData> logger;
    return 0;
}
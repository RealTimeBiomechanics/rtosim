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

#include "rtosim/FlowControl.h"

namespace rtosim {

    FlowControl::FlowControl() {
        //initialisation from constructor gives some probles, so it's in the body
        runCondition_.store(true);
    }

    FlowControl::FlowControl(bool value) {
        //initialisation from constructor gives some probles, so it's in the body
        runCondition_.store(value);
    }

    bool FlowControl::getRunCondition() const {
        return runCondition_.load();
    }

    void FlowControl::setRunCondition(bool run) {
        runCondition_.store(run);
    }
}
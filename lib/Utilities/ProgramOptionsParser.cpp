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

#include "rtosim/ProgramOptionsParser.h"
#include <string>
#include <vector>
#include <sstream>

namespace rtosim {

    ProgramOptionsParser::ProgramOptionsParser(int argc, char **argv) {

        char** it = argv;
        char** end = argv + argc;
        for (it; it != end; ++it)
            po_.push_back(*it);
    }

    bool ProgramOptionsParser::exists(const std::string& option) const {

        return std::find(po_.begin(), po_.end(), option) != po_.end();
    }

    std::string ProgramOptionsParser::getParameter(const std::string& option) const {

        return getParameter<std::string>(option);
    }
}
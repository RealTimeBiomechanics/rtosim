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

#ifndef rtosim_ProgramOptionsParser_h
#define rtosim_ProgramOptionsParser_h

#include <string>
#include <vector>
#include <algorithm>

namespace rtosim {

    class ProgramOptionsParser {
    public:
        ProgramOptionsParser(int argc, char **argv);
        bool exists(const std::string& option) const;
        //get the parameter that follows the option flag
        template<typename T>
        T getParameter(const std::string& option) const;
        std::string getParameter(const std::string& option) const;
        bool empty() const { return po_.size() < 2; }

    private:
        std::vector<std::string> po_;
    };

    template<typename T>
    T ProgramOptionsParser::getParameter(const std::string& option) const {

        auto it(std::find(po_.begin(), po_.end(), option));
        T ans{ 0 };
        if (it != po_.end() && std::next(it) != po_.end()) {
            std::stringstream ss(*std::next(it));
            ss >> ans;
        }
        return ans;
    }
}

#endif
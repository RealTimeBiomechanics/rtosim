/* -------------------------------------------------------------------------- *
* CEINMS is a standalone toolbox for neuromusculoskeletal modelling and      *
* simulation. CEINMS can also be used as a plugin for OpenSim either         *
* through the OpenSim GUI or API. See https://simtk.org/home/ceinms and the  *
* NOTICE file for more information. CEINMS development was coordinated       *
* through Griffith University and supported by the Australian National       *
* Health and Medical Research Council (NHMRC), the US National Institutes of *
* Health (NIH), and the European Union Framework Programme 7 (EU FP7). Also  *
* see the PROJECTS file for more information about the funding projects.     *
*                                                                            *
* Copyright (c) 2010-2015 Griffith University and the Contributors           *
*                                                                            *
* CEINMS Contributors: C. Pizzolato, M. Reggiani, M. Sartori,                *
*                      E. Ceseracciu, and D.G. Lloyd                         *
*                                                                            *
* Author(s): C. Pizzolato                                                    *
*                                                                            *
* CEINMS is licensed under the Apache License, Version 2.0 (the "License").  *
* You may not use this file except in compliance with the License. You may   *
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.*
*                                                                            *
* Unless required by applicable law or agreed to in writing, software        *
* distributed under the License is distributed on an "AS IS" BASIS,          *
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
* See the License for the specific language governing permissions and        *
* limitations under the License.                                             *
* -------------------------------------------------------------------------- */

#ifndef rtosim_Mapper_h
#define rtosim_Mapper_h

#include <vector>
#include <string>
#include <iostream>

namespace rtosim {

    class Mapper {

    public:
        Mapper();
        Mapper(const std::vector<std::string>& inNames, const std::vector<std::string>& outNames);
        void setNames(const std::vector<std::string>& inNames, const std::vector<std::string>& outNames);
        template<typename T>
        std::vector<T> map(const std::vector<T>& in, T defaultValue = T{}) const; //the return object will be copy elided
        size_t getOutSize() const { return outSize_; }
        size_t getInSize() const { return inSize_; }
    private:
        size_t inSize_, outSize_;
        std::vector<int> inToOutMapping_; //using int because in case in > out, we can use -1 as not-map token
        void updateMapping();
    };

    template<typename T>
    std::vector<T> Mapper::map(const std::vector<T>& in, T defaultValue) const {

        if (in.size() != inSize_) {
            std::cout << "Mapper: provided " << in.size() << " values, espected " << inSize_ << std::endl;
            std::exit(EXIT_FAILURE);
        }
        //this is flawed, since it works with double, but may have problems with other types..
        //maybe add a second parameter that is the default value
        std::vector<T> out(outSize_, defaultValue);
        for (unsigned i(0); i < inToOutMapping_.size(); ++i) {
            int mapIdx(inToOutMapping_.at(i));
            if (mapIdx >= 0)
                out.at(mapIdx) = in.at(i);
        }
        return out;
    }
}

#endif
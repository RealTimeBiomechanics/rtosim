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

#ifndef rtosim_GeneralisedCoordinatesData_h
#define rtosim_GeneralisedCoordinatesData_h

#include <vector>

//TODO: add checks for different vecotr sizes and stuff
namespace rtosim {
    class GeneralisedCoordinatesData {

    public:

        GeneralisedCoordinatesData();
        GeneralisedCoordinatesData(size_t nCoordinates);
        size_t getNCoordinates() const;
        void setQ(const std::vector<double>& q);
        void setQd(const std::vector<double>& qd);
        void setQdd(const std::vector<double>& qdd);
        const std::vector<double>& getQ() const;
        const std::vector<double>& getQd() const;
        const std::vector<double>& getQdd() const;
        void getQ(std::vector<double>& q) const;
        void getQd(std::vector<double>& qd) const;
        void getQdd(std::vector<double>& qdd) const;

    private:

        size_t nCoordinates_;
        std::vector<double> q_, qd_, qdd_;
    };
}
#endif

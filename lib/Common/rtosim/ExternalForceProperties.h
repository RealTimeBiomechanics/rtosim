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

#ifndef rtosim_ExternalForceProperties_h
#define rtosim_ExternalForceProperties_h
#include <string>
#include <Simbody.h>

namespace rtosim {
    class ExternalForceProperties {
    public:
        ExternalForceProperties(const std::string& xmlFilename);
        ExternalForceProperties(const SimTK::Xml::Element& element);
        std::string getName() const;
        std::string getAppliedToBody() const;
        std::string getForceExpressedInBody() const;
        std::string getPointExpressedInBody() const;
        std::string getForceIdentifier() const;
        std::string getPointIdentifier() const;
        std::string getTorqueIdentifier() const;

        void print();

    private:
        void parseElement(SimTK::Xml::Element elt);
        std::string name_;
        std::string applied_to_body_;
        std::string force_expressed_in_body_;
        std::string point_expressed_in_body_;
        std::string force_identifier_;
        std::string point_identifier_;
        std::string torque_identifier_;
    };
}
#endif
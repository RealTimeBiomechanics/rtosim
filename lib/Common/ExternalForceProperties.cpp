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

#include "rtosim/ExternalForceProperties.h"

namespace rtosim {

    ExternalForceProperties::ExternalForceProperties(const SimTK::Xml::Element& element) {

        parseElement(element);
    }

    void ExternalForceProperties::parseElement(SimTK::Xml::Element elt) {

        SimTK::Xml::attribute_iterator ap = elt.attribute_begin();
        if (ap->getName() == "name")
            name_ = ap->getValue();

        SimTK::Xml::node_iterator p = elt.node_begin();
        for (; p != elt.node_end(); ++p) {
            if (p->getNodeType() == SimTK::Xml::ElementNode) {
                auto e(SimTK::Xml::Element::getAs(*p));
                if (e.getElementTag() == "applied_to_body") applied_to_body_ = e.getValue();
                if (e.getElementTag() == "force_expressed_in_body") force_expressed_in_body_ = e.getValue();
                if (e.getElementTag() == "point_expressed_in_body") point_expressed_in_body_ = e.getValue();
                if (e.getElementTag() == "force_identifier") force_identifier_ = e.getValue();
                if (e.getElementTag() == "point_identifier") point_identifier_ = e.getValue();
                if (e.getElementTag() == "torque_identifier") torque_identifier_ = e.getValue();
            }
        }
    }

    std::string ExternalForceProperties::getName() const {
        return name_;
    }

    std::string ExternalForceProperties::getAppliedToBody() const {
        return applied_to_body_;
    }

    std::string ExternalForceProperties::getForceExpressedInBody() const{
        return force_expressed_in_body_;
    }

    std::string ExternalForceProperties::getPointExpressedInBody() const {
        return point_expressed_in_body_;
    }

    std::string ExternalForceProperties::getForceIdentifier() const {
        return force_identifier_;
    }

    std::string ExternalForceProperties::getPointIdentifier() const {
        return point_identifier_;
    }

    std::string ExternalForceProperties::getTorqueIdentifier() const {
        return torque_identifier_;
    }

    void ExternalForceProperties::print() {

        std::cout
            << "applied_to_body: " << applied_to_body_ << std::endl
            << "force_expressed_in_body: " << force_expressed_in_body_ << std::endl
            << "point_expressed_in_body: " << point_expressed_in_body_ << std::endl
            << "force_identifier: " << force_identifier_ << std::endl
            << "point_identifier: " << point_identifier_ << std::endl
            << "torque_identifier: " << torque_identifier_ << std::endl;
    }
}
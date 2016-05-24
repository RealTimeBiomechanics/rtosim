#include "rtosim/ExternalLoadProperties.h"
using SimTK::Xml;

namespace rtosim {

    ExternalLoadProperties::ExternalLoadProperties(const std::string& xmlFilename)
        :xmlFilename_(xmlFilename) {

        SimTK::Xml extLoadXml(xmlFilename_);
        parseXml(extLoadXml.getRootElement());
        datafile_ = getCorrectFilePath(datafile_);
        external_loads_model_kinematics_file_ = getCorrectFilePath(external_loads_model_kinematics_file_);
    }

    std::string ExternalLoadProperties::getCorrectFilePath(std::string file) const {

        if (!SimTK::Pathname::fileExists(file)) {
            bool isAbsolute(false);
            std::string currentDir, filename, extension;
            SimTK::Pathname::deconstructPathname(xmlFilename_, isAbsolute, currentDir, filename, extension);
            file = SimTK::Pathname::getAbsolutePathname(currentDir + SimTK::Pathname::getPathSeparator() + file);
        }
        return file;
    }

    void ExternalLoadProperties::parseXml(Xml::Element elt) {

        SimTK::Xml::element_iterator eIt(elt.element_begin());
        if (eIt->getElementTag() == "ExternalLoads")
            parseExternalLoads(*eIt);
    }

    void ExternalLoadProperties::parseExternalLoads(Xml::Element elt) {

        Xml::attribute_iterator ap = elt.attribute_begin();
        if (ap->getName() == "name")
            name_ = ap->getValue();

        Xml::element_iterator eIt(elt.element_begin());
        for (eIt; eIt != elt.element_end(); ++eIt) {
            if (eIt->getElementTag() == "objects") parseObjects(*eIt);
            if (eIt->getElementTag() == "datafile") datafile_ = eIt->getValue();
            if (eIt->getElementTag() == "external_loads_model_kinematics_file") external_loads_model_kinematics_file_ = eIt->getValue();
            if (eIt->getElementTag() == "lowpass_cutoff_frequency_for_load_kinematics") lowpass_cutoff_frequency_for_load_kinematics_ = eIt->getValueAs<double>();
        }
    }

    void ExternalLoadProperties::parseObjects(Xml::Element elt) {
        Xml::element_iterator eIt(elt.element_begin());
        for (eIt; eIt != elt.element_end(); ++eIt) {
            if (eIt->getElementTag() == "ExternalForce") {
                ExternalForceProperties extForce(*eIt);
                externalForcesProperties_.push_back(extForce);
            }
        }
    }

    std::size_t ExternalLoadProperties::getExternalForcesSize() const{

        return externalForcesProperties_.size();
    }

    std::vector<ExternalForceProperties> ExternalLoadProperties::getExternalForces() const {

        return externalForcesProperties_;
    }

    std::vector<std::string> ExternalLoadProperties::getExternalForcesNames() const {

        std::vector<std::string> names;
        for (auto& e : externalForcesProperties_)
            names.emplace_back(e.getName());
        return names;
    }

    std::string ExternalLoadProperties::getName() const {
        return name_;
    }

    std::string ExternalLoadProperties::getDatafile() const {

        return datafile_;
    }

    std::string ExternalLoadProperties::getKinematicsFile() const {

        return external_loads_model_kinematics_file_;
    }

    double ExternalLoadProperties::getCutoffFrequency() const {

        return lowpass_cutoff_frequency_for_load_kinematics_;
    }

    std::string ExternalLoadProperties::getForceIdentifier(const std::string& forceName) const {

        std::string forceIdentifier;
        for (auto& e : externalForcesProperties_) {
            if (e.getName() == forceName)
                forceIdentifier = e.getForceIdentifier();
        }
        return forceIdentifier;
    }

    std::string ExternalLoadProperties::getPointIdentifier(const std::string& forceName) const {

        std::string pointIdentifier;
        for (auto& e : externalForcesProperties_) {
            if (e.getName() == forceName)
                pointIdentifier = e.getPointIdentifier();
        }
        return pointIdentifier;
    }

    std::string ExternalLoadProperties::getTorqueIdentifier(const std::string& forceName) const {

        std::string torqueIdentifier;
        for (auto& e : externalForcesProperties_) {
            if (e.getName() == forceName)
                torqueIdentifier = e.getTorqueIdentifier();
        }
        return torqueIdentifier;
    }
}
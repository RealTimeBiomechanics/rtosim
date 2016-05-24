#ifndef rtosim_ExternalLoadProperties_h
#define rtosim_ExternalLoadProperties_h

#include "rtosim/ExternalForceProperties.h"
#include <Simbody.h>
#include <string>
#include <vector>

namespace rtosim {
    class ExternalLoadProperties {
    public:
        ExternalLoadProperties(const std::string& xmlFilename);
        std::size_t getExternalForcesSize() const;
        std::vector<ExternalForceProperties> getExternalForces() const;
        std::vector<std::string> getExternalForcesNames() const;
        std::string getForceIdentifier(const std::string& forceName) const;
        std::string getPointIdentifier(const std::string& forceName) const;
        std::string getTorqueIdentifier(const std::string& forceName) const;
        std::string getName() const;
        std::string getDatafile() const;
        std::string getKinematicsFile() const;
        double getCutoffFrequency() const;

    private:
        void parseXml(SimTK::Xml::Element elt);
        void parseExternalLoads(SimTK::Xml::Element elt);
        void parseObjects(SimTK::Xml::Element elt);
        std::vector < ExternalForceProperties > externalForcesProperties_;
        std::string name_;
        std::string datafile_;
        std::string external_loads_model_kinematics_file_;
        double lowpass_cutoff_frequency_for_load_kinematics_;
    };
}

#endif
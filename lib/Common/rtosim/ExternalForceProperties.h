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
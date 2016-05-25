#ifndef rtosim_ExternalForceFromQueue_h
#define rtosim_ExternalForceFromQueue_h

#include "rtosim/queue/SingleExternalForceQueue.h"
#include "rtosim/ExternalForceProperties.h"
#include <OpenSim/OpenSim.h>
#include <Simbody.h>

namespace rtosim {

    //thic class reads the force data from a Queue rather than a storage
    //This can be connected to an OpenSim model
    class ExternalForceFromQueue : public OpenSim::ExternalForce{
    public:
        ExternalForceFromQueue() = delete;
        ExternalForceFromQueue(ExternalForceFromQueue&) = delete;
        ExternalForceFromQueue(
            SingleExternalForceQueue* externalForceQueue,
            const std::string& forceIdentifier,
            const std::string& pointIdentifier,
            const std::string& torqueIdentifier,
            const std::string& appliedToBodyName,
            const std::string& forceExpressedInBodyName,
            const std::string& pointExpressedInBodyName);
        ExternalForceFromQueue(
            SingleExternalForceQueue* externalForceQueue,
            SimTK::Xml::Element& aNode);
        ExternalForceFromQueue(
            SingleExternalForceQueue* externalForceQueue,
            const ExternalForceProperties& externalForceProperties);

        virtual ~ExternalForceFromQueue();

    protected:
        virtual void computeForce(const SimTK::State& state,
            SimTK::Vector_<SimTK::SpatialVec>& bodyForces,
            SimTK::Vector& generalizedForces) const override;
        void connectToModel(OpenSim::Model& model) override;

    private:
        ExternalForceData getExternalForceDataFromPastValues(double time) const;
        ExternalForceData getExternalForceDataFromQueue(double time) const;

        //need to use raw pointers because opensim deletes stuff for me.. and it's a mess
        bool isSubscribed;
        SingleExternalForceQueue* externalForceQueue_;
        mutable std::list<SingleExternalForceFrame> pastExternalForceFrames_;
        static const unsigned MaxStorageLength_ = 1000;
        mutable double lastTime_;
        SimTK::ReferencePtr<const OpenSim::Body> appliedToBody_;
        SimTK::ReferencePtr<const OpenSim::Body> forceExpressedInBody_;
        SimTK::ReferencePtr<const OpenSim::Body> pointExpressedInBody_;
        mutable bool runCondition_;
    };
}

#endif
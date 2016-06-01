#ifndef rtosim_AdaptiveCop_h
#define rtosim_AdaptiveCop_h

#include "rtosim/queue/MarkerSetQueue.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/ExternalLoadProperties.h"
#include <map>

namespace rtosim{

    //When the foot is not in contact with the force plate, the COP is 
    //normally set to 0 by the input device. AdaptiveCop modify the value of the COP 
    //when the foot is not in contact with the force plate, and sets it equal to the 
    //projection of the marker defined in forcePlatesToMarkerNames on the force plate. 
    //In this way, the COP follows the foot. 
    //This helps to reduce the rise-time delay introduced 
    //filtering the COP.
    class AdaptiveCop {

    public:
        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::map<std::string, std::string>& forcePlatesToMarkerNames,
            const std::vector<std::string>& markerNames);

        //when using this constructor, the marker that is used to 
        //adapt the COP corresponds to the most posterior marker 
        //connected to the body defined in the applied_to_body tag in 
        //externalLoadsFilename
        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& externalLoadsFilename);

        AdaptiveCop(
            MarkerSetQueue& inputMarkerSetQueue,
            MultipleExternalForcesQueue& inputExternalForcesQueue,
            MultipleExternalForcesQueue& outputExternalForcesQueue,
            Concurrency::Latch& doneWithSubscriptions,
            Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::vector<ExternalForceProperties>& externalForcesProperties);

        void operator()();

    private:
        void updateForcePlatesToMarkerIndex(const std::vector<ExternalForceProperties>& externalForcesProperties, const std::string& osimModelFilename);
        void adaptCop(ExternalForceData& forceData, const MarkerData& markerData) const;
        MarkerSetQueue& inputMarkerSetQueue_;
        MultipleExternalForcesQueue& outputExternalForcesQueue_;
        MultipleExternalForcesQueue& inputExternalForcesQueue_;
        Concurrency::Latch& doneWithSubscriptions_;
        Concurrency::Latch& doneWithExecution_;
        std::map<std::string, unsigned> forcePlatesToMarkerIndex_;
    };
}

#endif
#ifndef rtosim_MarkersReferenceFromQueue_h
#define rtosim_MarkersReferenceFromQueue_h

#include <vector>
#include <string>

#include <OpenSim/Simulation/MarkersReference.h>
#include <SimTKcommon.h>
#include "rtosim/ThreadPoolJobs.h"
#include "rtosim/MarkerData.h"
#include "rtosim/queue/MarkerSetQueue.h"

namespace rtosim{

    class MarkersReferenceFromQueue : public OpenSim::MarkersReference {

    public:
        MarkersReferenceFromQueue(ThreadPoolJobs<MarkerSetFrame>& inputMarkerSetFrameQueue, const std::vector<std::string>& markerNames, const std::vector<double>& markerWeigths);
        virtual ~MarkersReferenceFromQueue() {}
        virtual void getValues(const SimTK::State &s, SimTK::Array_<SimTK::Vec3> &values) const override;
        virtual void getWeights(const SimTK::State &s, SimTK::Array_<double> &weights) const override;
        virtual const SimTK::Array_<std::string>& getNames() const override;
        double getCurrentTime() { return time_; }
        bool isEndOfData() const;
        void purgeCurrentFrame();
    private:
        ThreadPoolJobs<MarkerSetFrame>& inputMarkerSetFrameQueue_;
        MarkerSetFrame getPastFrame(double time) const;
        MarkerSetFrame getFrameFromQueue() const;

        unsigned static const MaxFramesToStore = 1000;
        mutable std::list<MarkerSetFrame> pastFrames_;
        mutable double lastQueueTime_;
        mutable double time_;
        SimTK::Array_<std::string> markerNames_;
        std::vector<double> markerWeigths_;
    };
}

#endif

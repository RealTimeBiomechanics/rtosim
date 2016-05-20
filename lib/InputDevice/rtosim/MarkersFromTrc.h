#ifndef rtosim_MarkersFromTrc_h
#define rtosim_MarkersFromTrc_h

#include "rtosim/concurrency/Latch.h"
#include "rtosim/queue/MarkerSetQueue.h"
#include <string>
#include <vector>
#include <OpenSim/OpenSim.h>

namespace rtosim{

    class MarkersFromTrc{

    public:
        MarkersFromTrc() = delete;
        MarkersFromTrc(MarkersFromTrc&) = delete;
        MarkersFromTrc(
            MarkerSetQueue& outputMarkerSetQueue,
            rtosim::Concurrency::Latch& doneWithSubscriptions,
            rtosim::Concurrency::Latch& doneWithExecution,
            const std::string& osimModelFilename,
            const std::string& trcFilename,
            bool loop = true);
        void setOutputFrequency(double frequency);
        void setSpeedFactor(double speedFactor);
        void setFramesToSkip(unsigned n);
        void operator()();
        virtual ~MarkersFromTrc();

    private:
        void sendEndOfData();
        unsigned getSleepTime() const;
        void runLoop();
        void run();
        unsigned framesToSkip_;
        double speedFactor_;
        unsigned sampleFrequency_;
        std::vector<MarkerSetFrame> frames_;
        MarkerSetQueue& outputMarkerSetQueue_;
        rtosim::Concurrency::Latch& doneWithSubscriptions_;
        rtosim::Concurrency::Latch& doneWithExecution_;
        std::string trcFilename_;
        OpenSim::Model model_;
        std::size_t noMarkers_;
        bool loop_;
    };
}

#endif

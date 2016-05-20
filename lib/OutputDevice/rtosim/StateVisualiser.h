#ifndef rtosim_StateVisualiser_h
#define rtosim_StateVisualiser_h

#include <string>
#include <OpenSim/OpenSim.h>
#include "rtosim/concurrency/Concurrency.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"

namespace rtosim{

    class ShowInfo : public SimTK::DecorationGenerator {
    public:
        ShowInfo() : text_(""){}
        void setText(const std::string& text) {
            text_ = text;
        }
        void generateDecorations(const SimTK::State& state,
            SimTK::Array_<SimTK::DecorativeGeometry>& geometry) override
        {
            SimTK::DecorativeText info;
            info.setIsScreenText(true);
            info.setText(text_);
            geometry.push_back(info);
        }

    private:
        std::string text_;
    };

    class StateVisualiser {
    public:
        StateVisualiser(
            rtosim::GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue,
            const std::string& osimModelFilename);
        StateVisualiser(StateVisualiser&) = delete;
        virtual ~StateVisualiser() {}
        void operator()();
    private:
        OpenSim::Model model_;
        rtosim::GeneralisedCoordinatesQueue& inputGeneralisedCoordinatesQueue_;
    };
}
#endif

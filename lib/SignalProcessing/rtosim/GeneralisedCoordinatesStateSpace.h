#ifndef rtosim_GeneralisedCoordinatesStateSpace_h
#define rtosim_GeneralisedCoordinatesStateSpace_h

#include "rtosim/Filter/StateSpaceFilter.h"
#include "rtosim/GeneralisedCoordinatesData.h"
#include "rtosim/queue/GeneralisedCoordinatesQueue.h"
#include <vector>

namespace rtosim {

    class GeneralisedCoordinatesStateSpace {

    public:
        GeneralisedCoordinatesStateSpace(double fc, size_t dimension);
        rtosim::GeneralisedCoordinatesFrame filter(const rtosim::GeneralisedCoordinatesFrame& frame);
        rtosim::GeneralisedCoordinatesFrame operator()(const rtosim::GeneralisedCoordinatesFrame& frame) { return filter(frame); }
    private:
        size_t dim_;
        double fc_;
        std::vector< Filter::StateSpaceFilter<double> > generalisedCoordinatesFilter_;
    };
}

#endif

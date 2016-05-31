#ifndef rtosim_MultipleExternalForcesDataFilterStateSpace_h
#define rtosim_MultipleExternalForcesDataFilterStateSpace_h

#include "rtosim/Filter/StateSpaceFilter.h"
#include "rtosim/ExternalForceDataFilterStateSpace.h"
#include "rtosim/MultipleExternalForcesDataFilterStateSpace.h"
#include "rtosim/queue/MultipleExternalForcesQueue.h"
#include <vector>

namespace rtosim {

    class MultipleExternalForcesDataFilterStateSpace {
    public:
        MultipleExternalForcesDataFilterStateSpace(double fc, double forceThreshold, size_t dimension);
        MultipleExternalForcesFrame filter(const MultipleExternalForcesFrame& frame);
        MultipleExternalForcesFrame operator()(const MultipleExternalForcesFrame& frame) { return filter(frame); }
    private:
        size_t dim_;
        std::vector< ExternalForceDataFilterStateSpace > multipleExternalForcesDataFilters_;
    };
}

#endif
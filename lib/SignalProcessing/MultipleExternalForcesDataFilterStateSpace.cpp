#include "rtosim/MultipleExternalForcesDataFilterStateSpace.h"
#include "rtosim/SignalProcessingCommon.h"

namespace rtosim {

    MultipleExternalForcesDataFilterStateSpace::
        MultipleExternalForcesDataFilterStateSpace(
            double fc, 
            double forceThreshold, 
            size_t dimension):
            dim_(dimension),
            multipleExternalForcesDataFilters_(dimension, { fc, forceThreshold }) 
            {}

    MultipleExternalForcesFrame MultipleExternalForcesDataFilterStateSpace::filter(const MultipleExternalForcesFrame& frame) {

        MultipleExternalForcesFrame ans;
        ans.time = frame.time;
        for (unsigned i(0); i < frame.data.size(); ++i)
            ans.data.push_back(multipleExternalForcesDataFilters_.at(i).filter(frame.data.at(i), frame.time));
        return ans;
    }
}
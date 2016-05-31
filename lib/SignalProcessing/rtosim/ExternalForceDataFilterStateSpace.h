#ifndef rtosim_ExternalForceDataFilterStateSpace_h
#define rtosim_ExternalForceDataFilterStateSpace_h

#include "rtosim/Filter/StateSpaceFilter.h"
#include "rtosim/ExternalForceData.h"
#include "rtosim/queue/SingleExternalForceQueue.h"
#include <vector>

namespace rtosim {
    class ExternalForceDataFilterStateSpace {

    public:
        ExternalForceDataFilterStateSpace(double fc, double forceThreshold = 40);
        ExternalForceData filter(const ExternalForceData& data, double time);
        ExternalForceData operator()(const ExternalForceData& data, double time) { return filter(data, time); }
    private:
        SimTK::Vec3 filter(const SimTK::Vec3& vec, double time, std::vector< Filter::StateSpaceFilter<double> > & f);
        double fc_;
        size_t dim_;
        std::vector< Filter::StateSpaceFilter<double> > grfFilter_;
        std::vector< Filter::StateSpaceFilter<double> > copFilter_;
        std::vector< Filter::StateSpaceFilter<double> > momentsFilter_;
        std::vector< Filter::StateSpaceFilter<double> > torquesFilter_;
        double forceThreshold_;
    };
}

#endif
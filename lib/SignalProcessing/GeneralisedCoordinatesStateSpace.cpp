#include "rtosim/GeneralisedCoordinatesStateSpace.h"
#include "rtosim/SignalProcessingCommon.h"
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

namespace rtosim {

    GeneralisedCoordinatesStateSpace::GeneralisedCoordinatesStateSpace(double fc, size_t dimension) :
        dim_(dimension),
        fc_(fc)        {
        generalisedCoordinatesFilter_.resize(dimension);
    }

    rtosim::GeneralisedCoordinatesFrame GeneralisedCoordinatesStateSpace::filter(const rtosim::GeneralisedCoordinatesFrame& frame) {

        if (frame.data.getNCoordinates() != dim_) {
            cout << "GeneralisedCoordinatesDataFilter: number of coordinates differs from number of fileter.\n Exit.";
            //todo: add a graceful exit
            std::exit(EXIT_FAILURE);
        }
        auto filteredData(frame.data);

        std::vector<double> rawQ(frame.data.getQ());
        std::vector<double> q, qd, qdd;
        for (unsigned i(0); i < dim_; ++i) {
            generalisedCoordinatesFilter_.at(i).filter(rawQ.at(i), frame.time, fc_);
            q.push_back(generalisedCoordinatesFilter_.at(i).getFilteredValue());
            qd.push_back(generalisedCoordinatesFilter_.at(i).getFilteredFirstDerivative());
            qdd.push_back(generalisedCoordinatesFilter_.at(i).getFilteredSecondDerivative());
        }

        filteredData.setQ(q);
        filteredData.setQd(qd);
        filteredData.setQdd(qdd);

        return{ frame.time, filteredData };
    }
}
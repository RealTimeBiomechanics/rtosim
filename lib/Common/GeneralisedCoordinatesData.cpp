#include "rtosim/GeneralisedCoordinatesData.h"

namespace rtosim {

    GeneralisedCoordinatesData::GeneralisedCoordinatesData() :
        nCoordinates_(0)
    {}

    GeneralisedCoordinatesData::GeneralisedCoordinatesData(std::size_t nCoordinates) :
        nCoordinates_(nCoordinates),
        q_(nCoordinates, .0),
        qd_(nCoordinates, .0),
        qdd_(nCoordinates, .0)
    {}

    std::size_t GeneralisedCoordinatesData::getNCoordinates() const {

        return nCoordinates_;
    }

    void GeneralisedCoordinatesData::setQ(const std::vector<double>& q) {

        nCoordinates_ = q.size();
        q_ = q;
    }

    void GeneralisedCoordinatesData::setQd(const std::vector<double>& qd) {

        qd_ = qd;
    }

    void GeneralisedCoordinatesData::setQdd(const std::vector<double>& qdd) {

        qdd_ = qdd;
    }

    const std::vector<double>& GeneralisedCoordinatesData::getQ() const {

        return q_;
    }

    const std::vector<double>& GeneralisedCoordinatesData::getQd() const {

        return qd_;
    }

    const std::vector<double>& GeneralisedCoordinatesData::getQdd() const {

        return qdd_;
    }

    void GeneralisedCoordinatesData::getQ(std::vector<double>& q) const {

        q = q_;
    }

    void GeneralisedCoordinatesData::getQd(std::vector<double>& qd) const {

        qd = qd_;
    }

    void GeneralisedCoordinatesData::getQdd(std::vector<double>& qdd) const {

        qdd = qdd_;
    }
};
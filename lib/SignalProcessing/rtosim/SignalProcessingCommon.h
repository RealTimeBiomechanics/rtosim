#ifndef rtosim_SignalProcessingCommon_h
#define rtosim_SignalProcessingCommon_h

#include <Simbody.h>
#include <vector>

namespace rtosim {
    namespace SignalProcessingCommon {

        template<typename T, typename U>
        T filter(T& value, U& filt);

        template<typename U>
        SimTK::Vec3 filter(const SimTK::Vec3& vec, U& filt);

        //so I can filter both vector<Vec3> and vector<double>
        template<typename T, typename U>
        std::vector<T> filter(const std::vector<T>& v, std::vector<U>& filt);

        template<typename T>
        void resetFilters(T& filt);
    }
}

#include "SignalProcessingCommon.cpp"
#endif
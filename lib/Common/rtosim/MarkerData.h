#ifndef rtosim_MarkerData_h
#define rtosim_MarkerData_h

#include <string>
#include <Simbody.h>

namespace rtosim{

    class MarkerData {

    public:
        MarkerData();
        MarkerData(const SimTK::Vec3& coordinates);
        MarkerData(double x, double y, double z);
        SimTK::Vec3 getCoordinates() const;
        void setCoordinates(const SimTK::Vec3& coordinates);
        void setOccluded(bool isOccluded);
        bool getOccluded() const;

    private:
        std::string label_;
        bool isOccluded_;
        SimTK::Vec3 coordinates_;
    };
}

#endif

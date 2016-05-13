#ifndef rtosim_MarkerSetQueue_h
#define rtosim_MarkerSetQueue_h

#include "rtosim/queue/QueueData.h"
#include "rtosim/concurrency/Queue.h"
#include "rtosim/MarkerData.h"
#include <vector>

namespace rtosim {

    using MarkerSetData = std::vector < MarkerData > ;
    using MarkerSetFrame = rtosim::QueueData < MarkerSetData > ;
    using MarkerSetQueue = rtosim::Concurrency::Queue < MarkerSetFrame > ;
}

#endif
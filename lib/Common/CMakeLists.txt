#-------------------------------------------------------------------------- #
# Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                         #
#                                                                           #
# Licensed under the Apache License, Version 2.0 (the "License");           #
# you may not use this file except in compliance with the License.          #
# You may obtain a copy of the License at:                                  #
# http://www.apache.org/licenses/LICENSE-2.0                                #
#                                                                           #
# Unless required by applicable law or agreed to in writing, software       #
# distributed under the License is distributed on an "AS IS" BASIS,         #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  #
# See the License for the specific language governing permissions and       #
# limitations under the License.                                            #
# --------------------------------------------------------------------------#

# List source files
set(Common_HEADERS
    rtosim/QueueData.h
    rtosim/MarkerData.h
    rtosim/GeneralisedCoordinatesData.h
    rtosim/EndOfData.h
    rtosim/QueuesSync.h
    rtosim/FlowControl.h
    rtosim/Common.h
    rtosim/ExternalForceData.h
    rtosim/ExternalLoadProperties.h
    rtosim/ExternalForceProperties.h
    rtosim/TimeDifference.h)

set(Common_QUEUES_HEADERS
    rtosim/queue/MarkerSetQueue.h
    rtosim/queue/GeneralisedCoordinatesQueue.h
    rtosim/queue/MultipleExternalForcesQueue.h
    rtosim/queue/ExternalTorquesQueue.h
    rtosim/queue/ScalarVectorQueue.h
    rtosim/queue/SingleExternalForceQueue.h
    rtosim/queue/OrientationSetQueue.h	
) 
    

set(Common_SOURCES
    MarkerData.cpp
    GeneralisedCoordinatesData.cpp
    ExternalForceData.cpp
    FlowControl.cpp
    ExternalLoadProperties.cpp
    ExternalForceProperties.cpp)

set(Common_TEMPLATE_IMPLEMENTATION
    TimeDifference.cpp)


set_source_files_properties( ${Common_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${Common_HEADERS} ${Common_QUEUES_HEADERS})
source_group("Sources" FILES ${Common_SOURCES} ${Common_TEMPLATE_IMPLEMENTATION} )

# Define target
add_library(Common ${Common_SOURCES} ${Common_HEADERS} ${Common_QUEUES_HEADERS} ${Common_TEMPLATE_IMPLEMENTATION})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Common rtb::Concurrency ${OPENSIMSIMBODY_LIBRARIES})
target_include_directories(Common  PRIVATE ${OPENSIMSIMBODY_INCLUDE_DIRS}
                                   PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                   $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(FILES ${Common_HEADERS} ${Common_TEMPLATE_IMPLEMENTATIONS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(FILES ${Common_QUEUES_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim/queue)

install(TARGETS Common EXPORT RTOSIM_Exports DESTINATION ${CMAKE_INSTALL_LIBDIR})

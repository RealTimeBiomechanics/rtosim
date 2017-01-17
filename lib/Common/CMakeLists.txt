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
    rtosim/TimeDifference.h
    )

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
    ExternalForceProperties.cpp
    )

set(Common_TEMPLATE_IMPLEMENTATION	
    TimeDifference.cpp)


set_source_files_properties( ${Common_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${Common_HEADERS} ${Common_QUEUES_HEADERS})
source_group("Sources" FILES ${Common_SOURCES} ${Common_TEMPLATE_IMPLEMENTATION} )

# Define target
add_library(Common STATIC ${Common_SOURCES} ${Common_HEADERS} ${Common_QUEUES_HEADERS} ${Common_TEMPLATE_IMPLEMENTATION})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Common  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Concurrency_INCLUDE_DIRS}   ${IKExtended_HEADERS} ${SIMTK_INCLUDE_DIRS} ${OPENSIM_INCLUDE_DIRS})
# Save path to include directory for "internal" usage
set(rtosim_Common_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for library of common files")

install(FILES ${Common_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS Common
        EXPORT RtosimTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
        )

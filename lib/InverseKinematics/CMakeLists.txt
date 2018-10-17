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
set(InverseKinematics_SOURCES
                        QueueToInverseKinametics.cpp
                        IKSequencer.cpp
                        IKSolverParallel.cpp
                        JobsCreator.cpp
                        MarkersReferenceFromQueue.cpp
                        TimeSequence.cpp)

set(InverseKinematics_HEADERS
                        rtosim/InverseKinematics.h
                        rtosim/QueueToInverseKinametics.h
                        rtosim/IKSequencer.h
                        rtosim/IKSolverParallel.h
                        rtosim/IKoutputs.h
                        rtosim/JobsCreator.h
                        rtosim/MarkersReferenceFromQueue.h
                        rtosim/ThreadPoolJobs.h
                        rtosim/TimeSequence.h)

set(InverseKinematics_TEMPLATE_IMPLEMENTATION IKoutputs.cpp ThreadPoolJobs.cpp)


set_source_files_properties( ${InverseKinematics_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${InverseKinematics_HEADERS})
source_group("Sources" FILES ${InverseKinematics_SOURCES}  ${InverseKinematics_TEMPLATE_IMPLEMENTATION})

# Define target
add_library(InverseKinematics
  ${InverseKinematics_SOURCES}
  ${InverseKinematics_HEADERS}
  ${InverseKinematics_TEMPLATE_IMPLEMENTATION})
target_link_libraries(InverseKinematics Common Utilities rtb::Concurrency ${OPENSIMSIMBODY_LIBRARIES})
target_include_directories(InverseKinematics PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                             ${OPENSIMSIMBODY_INCLUDE_DIRS})
if(WIN32)
    target_compile_definitions(InverseKinematics PUBLIC WIN32) #Hack to get OpenSim 3.3 to work
endif()
# Install

install(FILES ${InverseKinematics_HEADERS} ${InverseKinematics_TEMPLATE_IMPLEMENTATION} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS InverseKinematics
        EXPORT RTOSIM_Exports
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

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
set(InverseKinematicsOsens_SOURCES
                        QueueToInverseKinematicsOsens.cpp
                        IKSequencer.cpp 
						IKSolverParallelOsens.cpp 
						OrientationSensorsReferenceFromQueue.cpp 
						TimeSequence.cpp
						)

set(InverseKinematicsOsens_HEADERS
						rtosim/InverseKinematicsOsens.h
                        rtosim/QueueToInverseKinematicsOsens.h
                        rtosim/IKSequencer.h 
						rtosim/IKSolverParallelOsens.h 
						rtosim/IKoutputs.h
                        rtosim/JobsCreatorOsens.h 
						rtosim/OrientationSensorsReferenceFromQueue.h 
						rtosim/ThreadPoolJobs.h
                        rtosim/TimeSequence.h
						)

set(InverseKinematicsOsens_TEMPLATE_IMPLEMENTATION 
  IKoutputs.cpp 
  ThreadPoolJobs.cpp
  JobsCreatorOsens.cpp
)


set_source_files_properties( ${InverseKinematicsOsens_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${InverseKinematicsOsens_HEADERS})
source_group("Sources" FILES ${InverseKinematicsOsens_SOURCES}  ${InverseKinematicsOsens_TEMPLATE_IMPLEMENTATION})

# Define target
add_library(InverseKinematicsOsens STATIC
  ${InverseKinematicsOsens_SOURCES}
  ${InverseKinematicsOsens_HEADERS}
  ${InverseKinematicsOsens_TEMPLATE_IMPLEMENTATION}
)
target_link_libraries(InverseKinematicsOsens Common Utilities rtb::Concurrency ${OPENSIMSIMBODY_LIBRARIES}  ${IKExtended_LIBRARIES})
target_include_directories(InverseKinematicsOsens PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                             ${OPENSIMSIMBODY_INCLUDE_DIRS} 
                                             ${IKExtended_INCLUDE_DIRS})


if(WIN32)
    target_compile_definitions(InverseKinematicsOsens PUBLIC WIN32) #Hack to get OpenSim 3.3 to work
endif()
# Install

install(FILES ${InverseKinematicsOsens_HEADERS} ${InverseKinematicsOsens_TEMPLATE_IMPLEMENTATION} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS InverseKinematicsOsens
        EXPORT RTOSIM_Exports
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
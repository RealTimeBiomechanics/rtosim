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

target_include_directories(InverseKinematicsOsens PRIVATE
		${rtosim_Common_INCLUDE_DIRS}
		${rtosim_Concurrency_INCLUDE_DIRS}
		${rtosim_Utilities_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}
		PUBLIC
		${OPENSIM_INCLUDE_DIRS}
             ${SIMTK_INCLUDE_DIRS}
		    ${IKExtended_HEADERS}
                                    )

# Save path to include directory for "internal" usage
set(rtosim_InverseKinematicsOsens_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for InverseKinematics library")
# Install

install(FILES ${InverseKinematicsOsens_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS InverseKinematicsOsens
        EXPORT RtosimTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
        )


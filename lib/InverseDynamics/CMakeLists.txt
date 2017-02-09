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
set(InverseDynamics_SOURCES
                        QueuesToInverseDynamics.cpp
                        QueueToXForces.cpp
                        ExternalForceFromQueue.cpp
                        JointContactFromOpensim.cpp
						)

set(InverseDynamics_HEADERS
						rtosim/InverseDynamics.h
                        rtosim/QueuesToInverseDynamics.h
                        rtosim/QueueToXForces.h
                        rtosim/ExternalForceFromQueue.h
                        rtosim/JointContactFromOpensim.h
						)

set(InverseDynamics_TEMPLATE_IMPLEMENTATION )


set_source_files_properties( ${InverseDynamics_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${InverseDynamics_HEADERS})
source_group("Sources" FILES ${InverseDynamics_SOURCES}  ${InverseDynamics_TEMPLATE_IMPLEMENTATION})

# Define target
add_library(InverseDynamics STATIC
  ${InverseDynamics_SOURCES}
  ${InverseDynamics_HEADERS}
  ${InverseDynamics_TEMPLATE_IMPLEMENTATION}
)

target_include_directories(InverseDynamics PRIVATE
									         ${rtosim_Common_INCLUDE_DIRS}
								    	     ${rtosim_Concurrency_INCLUDE_DIRS}
										     ${rtosim_Utilities_INCLUDE_DIRS}
								    	     ${CMAKE_CURRENT_SOURCE_DIR}
                                             PUBLIC
                                             ${OPENSIMSIMBODY_INCLUDE_DIRS}
                                    )

TARGET_LINK_LIBRARIES(InverseDynamics Common)
# Save path to include directory for "internal" usage
set(rtosim_InverseDynamics_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for InverseDynamics library")
# Install

#install(FILES ${InverseKinematics_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl)
#install(TARGETS InverseKinematics
#        EXPORT VOSLTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
#        )

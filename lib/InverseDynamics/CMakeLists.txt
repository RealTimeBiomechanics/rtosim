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

source_group("Headers" FILES ${InverseDynamics_HEADERS})
source_group("Sources" FILES ${InverseDynamics_SOURCES})

# Define target
add_library(InverseDynamics STATIC
  ${InverseDynamics_SOURCES}
  ${InverseDynamics_HEADERS}
)

target_include_directories(InverseDynamics PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                                  ${OPENSIMSIMBODY_INCLUDE_DIRS})

target_link_libraries(InverseDynamics Common Utilities rtb::Concurrency ${OPENSIMSIMBODY_LIBRARIES})

# Install

install(FILES ${InverseDynamics_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS InverseDynamics
        EXPORT RTOSIM_Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

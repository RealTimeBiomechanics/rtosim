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
set(rtosim_HEADERS
    rtosim/rtosim.h)

source_group("Headers" FILES ${rtosim_HEADERS})
# Define target
add_library(rtosim INTERFACE)
add_custom_target(rtosim_IDE SOURCES ${rtosim_HEADERS})
target_link_libraries(rtosim INTERFACE Common InputDevice OutputDevice InverseKinematics
                             InverseDynamics SignalProcessing Utilities)
target_include_directories(rtosim INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
# Install
install(FILES ${rtosim_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS rtosim
        EXPORT RTOSIM_Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

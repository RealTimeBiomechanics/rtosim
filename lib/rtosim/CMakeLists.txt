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
	rtosim/rtosim.h
    )

source_group("Headers" FILES ${rtosim_HEADERS})
# Define target
add_library(rtosim STATIC ${rtosim_HEADERS})
set_target_properties(rtosim PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(rtosim rtb::Concurrency)
target_include_directories(rtosim PRIVATE 
                                ${CMAKE_CURRENT_SOURCE_DIR} 
                                ${rtosim_Common_INCLUDE_DIRS}
                                ${rtosim_InputDevice_INCLUDE_DIRS} 
                                ${rtosim_OutputDevice_INCLUDE_DIRS} 
                                ${rtosim_InverseKinematics_INCLUDE_DIRS}
                                ${rtosim_InverseDynamics_INCLUDE_DIRS}
                                ${rtosim_SignalProcessing_INCLUDE_DIRS}
                                ${rtosim_Utilities_INCLUDE_DIRS}
                                ${OPENSIMSIMBODY_INCLUDE_DIRS})
# Save path to include directory for "internal" usage
set(rtosim_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}     
	${rtosim_Utilities_INCLUDE_DIRS}
    ${rtosim_Common_INCLUDE_DIRS}
    ${rtosim_InputDevice_INCLUDE_DIRS}
	${rtosim_OutputDevice_INCLUDE_DIRS}
    ${rtosim_InverseKinematics_INCLUDE_DIRS}
	${rtosim_InverseDynamics_INCLUDE_DIRS}    
	${rtosim_SignalProcessing_INCLUDE_DIRS} CACHE INTERNAL "")

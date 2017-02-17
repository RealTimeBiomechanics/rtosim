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

set(SignalProcessing_HEADERS 
    rtosim/GeneralisedCoordinatesStateSpace.h
	rtosim/SignalProcessingCommon.h
    rtosim/SignalProcessing.h
    rtosim/QueueAdapter.h
    rtosim/ExternalForceDataFilterStateSpace.h
    rtosim/MultipleExternalForcesDataFilterStateSpace.h
    rtosim/AdaptiveCop.h
    )

set(SignalProcessing_SOURCES 
    GeneralisedCoordinatesStateSpace.cpp
    ExternalForceDataFilterStateSpace.cpp
    MultipleExternalForcesDataFilterStateSpace.cpp
    AdaptiveCop.cpp
    )
    
set(SignalProcessing_TEMPLATE_IMPLEMENTATIONS 
	SignalProcessingCommon.cpp
    QueueAdapter.cpp)

set_source_files_properties(
    ${SignalProcessing_TEMPLATE_IMPLEMENTATIONS} 
    PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("Header files" FILES ${SignalProcessing_HEADERS})
source_group("Source files" FILES ${SignalProcessing_SOURCES} ${SignalProcessing_SOURCES})

include_directories(include)
add_library(SignalProcessing
    ${SignalProcessing_HEADERS} 
    ${SignalProcessing_TEMPLATE_IMPLEMENTATIONS} 
    ${SignalProcessing_SOURCES} 
    )
target_link_libraries(SignalProcessing Common rtb::Filter)
target_include_directories(SignalProcessing 
	PRIVATE	
		${rtosim_Common_INCLUDE_DIRS}
		${rtosim_Utilities_INCLUDE_DIRS}
		${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC
        ${OPENSIMSIMBODY_INCLUDE_DIRS}
		)

set(rtosim_SignalProcessing_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE PATH "Include directory for SignalProcessing library" FORCE)
mark_as_advanced(rtosim_SignalProcessing_INCLUDE_DIRS)

#install(FILES ${Filter_HEADERS} ${Filter_TEMPLATE_IMPLEMENTATIONS}
 #       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl/Filter)


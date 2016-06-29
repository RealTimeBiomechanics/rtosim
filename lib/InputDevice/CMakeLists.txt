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

set(InputDevice_SOURCES 
    MarkersFromTrc.cpp
    ExternalForcesFromX.cpp
    ExternalForcesFromStorageFile.cpp
    GeneralisedCoordinatesFromX.cpp
    GeneralisedCoordinatesFromStorageFile.cpp
    )

set(InputDevice_HEADERS 
    rtosim/MarkersFromTrc.h
    rtosim/InputDevice.h
    rtosim/ExternalForcesFromX.h
    rtosim/ExternalForcesFromStorageFile.h
    rtosim/GeneralisedCoordinatesFromX.h
    rtosim/GeneralisedCoordinatesFromStorageFile.h
	)

SET(InputDevice_Nexus_SOURCES
	    DataFromNexus.cpp )

set(InputDevice_Nexus_HEADERS 
		rtosim/DataFromNexus.h )
	 
IF(${VICONSDK_FOUND})
	MESSAGE( STATUS "Using ViconSDK in InputDevice")
	SET(InputDevice_HEADERS ${InputDevice_HEADERS} ${InputDevice_Nexus_HEADERS})
	SET(InputDevice_SOURCES ${InputDevice_SOURCES} ${InputDevice_Nexus_SOURCES})
	add_definitions(INCLUDEVICONSDK)
ENDIF(${VICONSDK_FOUND})

source_group("Headers" FILES ${InputDevice_HEADERS})
source_group("Sources" FILES ${InputDevice_SOURCES})

# Define target
add_library(InputDevice STATIC
    ${InputDevice_SOURCES}
    ${InputDevice_HEADERS}
)

target_include_directories(InputDevice PRIVATE
                                ${rtosim_Concurrency_INCLUDE_DIRS}
								${rtosim_Common_INCLUDE_DIRS}
								${rtosim_Utilities_INCLUDE_DIRS} 
                                ${CMAKE_CURRENT_SOURCE_DIR}
								PUBLIC
								${SIMTK_INCLUDE_DIRS}
								${OPENSIM_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(InputDevice Common Utilities)
IF(${VICONSDK_FOUND})
	target_include_directories(InputDevice PUBLIC
								${ViconSDK_INCLUDE_DIRS}
	)
	TARGET_LINK_LIBRARIES(InputDevice ${ViconSDK_LIBRARIES})
ENDIF(${VICONSDK_FOUND})


# Save path to include directory for "internal" usage
set(rtosim_InputDevice_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for InputDevice library")
# Install

#install(FILES ${InputDevice_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl)
#install(TARGETS InputDevice
#        EXPORT VOSLTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
 #       )

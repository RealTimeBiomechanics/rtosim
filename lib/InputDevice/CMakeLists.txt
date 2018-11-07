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
    OrientationsFromMot.cpp
#	OrientationsFromNoraxonCsv.cpp
    OrientationsFromMobile.cpp
    ExternalForcesFromX.cpp
    ExternalForcesFromStorageFile.cpp
    GeneralisedCoordinatesFromX.cpp
	GeneralisedCoordinatesFromStorageFile.cpp
    )

set(InputDevice_HEADERS
    rtosim/MarkersFromTrc.h
    rtosim/OrientationsFromMot.h
#	rtosim/OrientationsFromNoraxonCsv.h
    rtosim/OrientationsFromMobile.h
    rtosim/InputDevice.h
    rtosim/ExternalForcesFromX.h
    rtosim/ExternalForcesFromStorageFile.h
    rtosim/GeneralisedCoordinatesFromX.h
    rtosim/GeneralisedCoordinatesFromStorageFile.h
	)

set(InputDevice_MR3_HEADERS
		rtosim/DataFromMR3.h
		rtosim/OrientationsFromMR3.h )

set(InputDevice_MR3_SOURCES
		OrientationsFromMR3.cpp
		DataFromMR3.cpp )

set(InputDevice_Nexus_HEADERS
        rtosim/DataFromNexus.h )

set(InputDevice_Nexus_SOURCES
        DataFromNexus.cpp )


if(${VICONSDK_FOUND})
    message( STATUS "Using ViconSDK in InputDevice")
    list(APPEND InputDevice_HEADERS ${InputDevice_Nexus_HEADERS})
    list(APPEND InputDevice_SOURCES ${InputDevice_Nexus_SOURCES})
endif(${VICONSDK_FOUND})

if(${MR3tools_FOUND})
    message( STATUS "Using MR3tools in InputDevice")
    list(APPEND InputDevice_HEADERS ${InputDevice_MR3_HEADERS})
    list(APPEND InputDevice_SOURCES ${InputDevice_MR3_SOURCES})
endif(${MR3tools_FOUND})

source_group("Headers" FILES ${InputDevice_HEADERS})
source_group("Sources" FILES ${InputDevice_SOURCES})

# Define target
add_library(InputDevice
    ${InputDevice_SOURCES}
    ${InputDevice_HEADERS})

target_include_directories(InputDevice PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                       $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                       ${OPENSIMSIMBODY_INCLUDE_DIRS})

target_link_libraries(InputDevice Common Utilities ${IKExtended_LIBRARIES} ${OPENSIMSIMBODY_LIBRARIES})
if(WIN32)
    target_compile_definitions(InputDevice PUBLIC WIN32) #Hack to get OpenSim 3.3 to work
endif()

if(${VICONSDK_FOUND})
    target_include_directories(InputDevice PUBLIC
                                ${ViconSDK_INCLUDE_DIRS}
    )
    target_link_libraries(InputDevice ${ViconSDK_LIBRARIES})
    target_compile_definitions(InputDevice PUBLIC INCLUDEVICONSDK)
endif(${VICONSDK_FOUND})

if(${MR3tools_FOUND})
    target_include_directories(InputDevice PUBLIC
                                ${MR3tools_INCLUDE_DIRS}
    )
    target_link_libraries(InputDevice MR3tools)
    target_compile_definitions(InputDevice PUBLIC INCLUDEMR3TOOLS)
endif(${MR3tools_FOUND})

# Install
install(FILES ${InputDevice_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS InputDevice
        EXPORT RTOSIM_Exports
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

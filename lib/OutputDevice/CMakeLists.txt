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

set(OutputDevice_SOURCES 
	StateVisualiser.cpp
    )

set(OutputDevice_HEADERS 
    rtosim/QueueToFileLogger.h
	rtosim/OutputDevice.h
    rtosim/FileLogger.h
    rtosim/FrameCounter.h
	rtosim/StateVisualiser.h
)

set(OutputDevice_TEMPLATE_IMPLEMENTATIONS 
    FileLogger.cpp
	QueueToFileLogger.cpp
    FrameCounter.cpp
)    
    
set_source_files_properties(${OutputDevice_TEMPLATE_IMPLEMENTATIONS} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("Headers" FILES ${OutputDevice_HEADERS})
source_group("Sources" FILES ${OutputDevice_SOURCES} ${OutputDevice_TEMPLATE_IMPLEMENTATIONS})

add_library(OutputDevice STATIC
    ${OutputDevice_SOURCES}
    ${OutputDevice_HEADERS}
	${OutputDevice_TEMPLATE_IMPLEMENTATIONS}
)

target_include_directories(
    OutputDevice 
    PRIVATE
        ${rtosim_Concurrency_INCLUDE_DIRS}
        ${rtosim_Common_INCLUDE_DIRS}
        ${rtosim_Utilities_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        ${OPENSIMSIMBODY_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(OutputDevice Common)#SharedData ${ViconSDK_LIBRARIES})
# Save path to include directory for "internal" usage
set(rtosim_OutputDevice_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for OutputDevice library")
# Install

#install(FILES ${OutputDevice_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl)
#install(TARGETS OutputDevice
#        EXPORT VOSLTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
 #       )

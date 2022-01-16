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
    StateVisualiser.cpp)

set(OutputDevice_HEADERS
    rtosim/QueueToFileLogger.h
    rtosim/OutputDevice.h
    rtosim/FileLogger.h
    rtosim/FrameCounter.h
    rtosim/StateVisualiser.h)

set(OutputDevice_TEMPLATE_IMPLEMENTATIONS
    FileLogger.cpp
    QueueToFileLogger.cpp
    FrameCounter.cpp)

set_source_files_properties(${OutputDevice_TEMPLATE_IMPLEMENTATIONS} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("Headers" FILES ${OutputDevice_HEADERS})
source_group("Sources" FILES ${OutputDevice_SOURCES} ${OutputDevice_TEMPLATE_IMPLEMENTATIONS})

add_library(OutputDevice
    ${OutputDevice_SOURCES}
    ${OutputDevice_HEADERS}
    ${OutputDevice_TEMPLATE_IMPLEMENTATIONS})

target_include_directories(OutputDevice PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(OutputDevice Common Utilities osimTools)
if(WIN32)
    target_compile_definitions(OutputDevice PUBLIC WIN32) #Hack to get OpenSim 3.3 to work
endif()
# Install

install(FILES ${OutputDevice_HEADERS} ${OutputDevice_TEMPLATE_IMPLEMENTATIONS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS OutputDevice
        EXPORT RTOSIM_Exports
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
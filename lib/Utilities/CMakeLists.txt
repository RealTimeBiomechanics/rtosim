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
set(Utilities_HEADERS
    rtosim/Utilities.h
    rtosim/ProgramOptionsParser.h
    rtosim/Mapper.h
    rtosim/ArrayConverter.h
    rtosim/StopWatch.h
    rtosim/FileSystem.h
    rtosim/OsimUtilities.h)

set(Utilities_SOURCES
    ProgramOptionsParser.cpp
    Mapper.cpp
    StopWatch.cpp
    FileSystem.cpp
    OsimUtilities.cpp)

source_group("Headers" FILES ${Utilities_HEADERS})
source_group("Sources" FILES ${Utilities_SOURCES})

# Define target
add_library(Utilities ${Utilities_SOURCES} ${Utilities_HEADERS})
target_link_libraries(Utilities PUBLIC ${OPENSIMSIMBODY_LIBRARIES} )
target_include_directories(Utilities PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
                                            ${OPENSIMSIMBODY_INCLUDE_DIRS}
                                            ${IKExtended_INCLUDE_DIRS})
if(WIN32)
    target_compile_definitions(Utilities PUBLIC WIN32) #Hack to get OpenSim 3.3 to work
endif()

install(FILES ${Utilities_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim)
install(TARGETS Utilities
        EXPORT RTOSIM_Exports
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

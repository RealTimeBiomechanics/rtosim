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
    rtosim/OsimUtilities.h
    )

set(Utilities_SOURCES 
    ProgramOptionsParser.cpp
    Mapper.cpp
    StopWatch.cpp
    FileSystem.cpp
    OsimUtilities.cpp
    )

set(Utilities_TEMPLATE_IMPLEMENTATION)


set_source_files_properties( ${Utilities_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${Utilities_HEADERS})
source_group("Sources" FILES ${Utilities_SOURCES} ${Utilities_TEMPLATE_IMPLEMENTATION} )

# Define target
add_library(Utilities STATIC ${Utilities_SOURCES} ${Utilities_HEADERS} ${Utilities_TEMPLATE_IMPLEMENTATION})
target_link_libraries(Utilities LINK_PUBLIC ${OPENSIMSIMBODY_LIBRARIES})
target_include_directories(Utilities PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${OPENSIMSIMBODY_INCLUDE_DIRS})

# Save path to include directory for "internal" usage
set(rtosim_Utilities_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for SignalProcessing files")

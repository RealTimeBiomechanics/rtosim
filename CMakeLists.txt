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

cmake_minimum_required (VERSION 3.0.0) # add_library(... INTERFACE is missing in older versions)
project(RTOSIM CXX)
set(RTOSIM_MAJOR_VERSION 1)
set(RTOSIM_MINOR_VERSION 0)
set(RTOSIM_PATCH_VERSION 0)

#include GNUInstallDirs to get canonical paths
include(GNUInstallDirs)

if(WIN32)
    set(CMAKE_CXX_FLAGS "/EHsc")
endif()
if(UNIX AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif(UNIX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(Concurrency REQUIRED)
find_package(Filter REQUIRED)
find_package(OpenSim REQUIRED)
find_package(ViconSDK QUIET)
if(${VICONSDK_FOUND})
	message(STATUS "ViconSDK found")
	add_definitions(-DINCLUDEVICONSDK)
else()
	message(STATUS "ViconSDK not found")
endif()


message(STATUS "OPENSIMSIMBODY_INCLUDE_DIRS: ${OPENSIMSIMBODY_INCLUDE_DIRS}")

add_definitions(/DBASE_DIR="${CMAKE_CURRENT_LIST_DIR}")

# Choose common build locations for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tests)

# Create RTOSIMConfig.cmake
set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/RTOSIM)

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/RTOSIMConfig.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/RTOSIMConfigForInstall.cmake
                              INSTALL_DESTINATION ${ConfigPackageLocation}
                              PATH_VARS CMAKE_INSTALL_INCLUDEDIR)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/RTOSIMConfigVersion.cmake
                                    VERSION 0.1
                                    COMPATIBILITY ExactVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RTOSIMConfigForInstall.cmake
              DESTINATION ${ConfigPackageLocation}
              RENAME RTOSIMConfig.cmake)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/RTOSIMConfigVersion.cmake
              DESTINATION ${ConfigPackageLocation})

install(EXPORT RTOSIM_Exports NAMESPACE rtb::
                              DESTINATION ${ConfigPackageLocation})

# Uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

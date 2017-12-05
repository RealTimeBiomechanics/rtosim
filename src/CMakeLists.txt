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

add_executable(rtosim_ik_from_file  mainInverseKinematicsFromFile.cpp)
target_link_libraries(rtosim_ik_from_file rtosim)

add_executable(rtosim_id_from_file mainInverseDynamicsfromFile.cpp)
target_link_libraries(rtosim_id_from_file rtosim)

add_executable(rtosim_ik_id_from_file mainInverseKinematicsInverseDynamicsFromFile.cpp)
target_link_libraries(rtosim_ik_id_from_file rtosim)




install(TARGETS rtosim_ik_from_file rtosim_id_from_file rtosim_ik_id_from_file
        EXPORT RTOSIM_Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(${VICONSDK_FOUND})
    add_executable(rtosim_ik_id_from_nexus mainInverseKinematicsInverseDynamicsFromNexus.cpp)
    target_link_libraries(rtosim_ik_id_from_nexus rtosim)

    install(TARGETS rtosim_ik_id_from_nexus
        EXPORT RTOSIM_Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(${VICONSDK_FOUND})


if(${MR3tools_FOUND})
    add_executable(test_data_from_MR3 mainTestDataFromMR3.cpp)
    target_link_libraries(test_data_from_MR3 rtosim MR3tools)

    add_executable(test_data_from_MR3_to_osim mainTestDataFromMR3toOsim.cpp)
    target_link_libraries(test_data_from_MR3_to_osim rtosim MR3tools)

    add_executable(fromMR3toFile mainFromMR3toFile.cpp)
    target_link_libraries(fromMR3toFile MR3tools rtosim)
endif(${MR3tools_FOUND})


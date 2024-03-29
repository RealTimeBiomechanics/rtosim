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

add_executable(rtosim_ik_from_file mainInverseKinematicsFromFile.cpp)
target_link_libraries(rtosim_ik_from_file rtosim)

add_executable(rtosim_id_from_file mainInverseDynamicsfromFile.cpp)
target_link_libraries(rtosim_id_from_file rtosim)

add_executable(rtosim_ik_id_from_file mainInverseKinematicsInverseDynamicsFromFile.cpp)
target_link_libraries(rtosim_ik_id_from_file rtosim)

set(targetsToExport rtosim_ik_from_file rtosim_id_from_file rtosim_ik_id_from_file )

if(${VICONSDK_FOUND})
    add_executable(rtosim_ik_id_from_nexus mainInverseKinematicsInverseDynamicsFromNexus.cpp)
    target_link_libraries(rtosim_ik_id_from_nexus rtosim)
    list(APPEND targetsToExport rtosim_ik_id_from_nexus)

endif(${VICONSDK_FOUND})

install(TARGETS ${targetsToExport}
        EXPORT RTOSIM_Targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})


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

include_directories(
    ${OPENSIM_INCLUDE_DIRS}
    ${SIMTK_INCLUDE_DIRS}
    ${rtosim_INCLUDE_DIRS}
    ${IKExtended_HEADERS}
	${MR3tools_HEADERS}
)
    
add_executable(rtosim_ik_from_file 
    mainInverseKinematicsFromFile.cpp
    )

target_link_libraries(rtosim_ik_from_file 
    ${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
    InverseKinematics
    SignalProcessing
    OutputDevice
    )
    

add_executable(rtosim_id_from_file 
    mainInverseDynamicsfromFile.cpp
    )

target_link_libraries(rtosim_id_from_file 
    ${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
    SignalProcessing
    InverseDynamics
    OutputDevice
    )
    
 add_executable(rtosim_ik_id_from_file 
    mainInverseKinematicsInverseDynamicsFromFile.cpp
    )

target_link_libraries(rtosim_ik_id_from_file 
    ${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
    SignalProcessing
    InverseKinematics
    InverseDynamics
    OutputDevice
    )   
    
    

IF(${VICONSDK_FOUND}) 
     add_executable(rtosim_ik_id_from_nexus 
    mainInverseKinematicsInverseDynamicsFromNexus.cpp
    )

target_link_libraries(rtosim_ik_id_from_nexus 
    ${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
    SignalProcessing
    InverseKinematics
    InverseDynamics
    OutputDevice
    )   
ENDIF(${VICONSDK_FOUND})

add_executable(rtosim_ik_osens_from_file 
    mainInverseKinematicsOsensFromFile.cpp
    )

target_link_libraries(rtosim_ik_osens_from_file 
#	${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
    SignalProcessing
    OutputDevice
	InverseKinematicsOsens
	IKExtended	
    )
    
add_executable(test_osens 
    mainTestOsens.cpp
    )

target_link_libraries(test_osens
    IKExtended
	${OPENSIM_LIBRARIES}
    
	Common
    Utilities
    Concurrency
    InputDevice
    InverseKinematicsOsens
  #  SignalProcessing
	
    OutputDevice
    )
    
#add_executable(test_osens_from_mobile 
#    mainRTOsensFromMobile.cpp
#    )

#target_link_libraries(test_osens_from_mobile
#    ${OPENSIM_LIBRARIES}
#    IKExtended
#    Common
#    Utilities
#    Concurrency
#    InputDevice
#    InverseKinematics
#    SignalProcessing
#    OutputDevice
#    )
#if(WIN32)
#    target_link_libraries(test_osens_from_mobile ws2_32)
#endif()

add_executable(test_osens_from_mobile_to_queue
    mainTestOsensFromMobileToQueue.cpp
    )

target_link_libraries(test_osens_from_mobile_to_queue
    ${OPENSIM_LIBRARIES}
    InverseKinematicsOsens
    Common
    Utilities
    Concurrency
    InputDevice
    InverseKinematics
    SignalProcessing
    OutputDevice
    )
#if(WIN32)
#    target_link_libraries(test_osens_from_mobile_to_queue ws2_32)
#endif()

#add_executable(rtosim_osens_ik_from_mobile
#    mainInverseKinematicsOsensFromMobile.cpp
#    )

#target_link_libraries(rtosim_osens_ik_from_mobile
#    ${OPENSIM_LIBRARIES}
#	InverseKinematicsOsens
#    IKExtended
#    Common
#    Utilities
#    Concurrency
#    InputDevice
#    SignalProcessing
#    OutputDevice
#    )


add_executable(test_data_from_MR3 mainTestDataFromMR3.cpp)

target_link_libraries(test_data_from_MR3
InputDevice
MR3tools
Concurrency
Utilities
Common
${OPENSIM_LIBRARIES})

add_executable(test_data_from_MR3_to_osim mainTestDataFromMR3toOsim.cpp)

target_link_libraries(test_data_from_MR3_to_osim
InputDevice
OutputDevice
MR3tools
Concurrency
Utilities
Common
IKExtended
InverseKinematicsOsens
${OPENSIM_LIBRARIES})

add_executable(fromMR3toFile mainFromMR3toFile.cpp)

target_link_libraries(fromMR3toFile
InputDevice
OutputDevice
MR3tools
Concurrency
Utilities
Common
${OPENSIM_LIBRARIES})

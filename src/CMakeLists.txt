include_directories(
    ${OPENSIM_INCLUDE_DIRS}
    ${SIMTK_INCLUDE_DIRS}
    ${rtosim_INCLUDE_DIRS}
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
    
    
add_executable(compareStorageFiles
    compareStorageFiles.cpp
    )
    
target_link_libraries(compareStorageFiles
    ${OPENSIM_LIBRARIES}
    )
 
    

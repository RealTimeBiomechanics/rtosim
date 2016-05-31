include_directories(
    ${OPENSIM_INCLUDE_DIRS}
    ${SIMTK_INCLUDE_DIRS}
	${rtosim_INCLUDE_DIRS}
)
    
add_executable(rtosim_ik 
    mainInverseKinematics.cpp
    )

target_link_libraries(rtosim_ik
    ${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
    InverseKinematics
	SignalProcessing
    OutputDevice
    )
    

add_executable(rtosim_id 
    mainInverseDynamics.cpp
    )

target_link_libraries(rtosim_id
    ${OPENSIM_LIBRARIES}
    Common
    Utilities
    Concurrency
    InputDevice
	SignalProcessing
	InverseDynamics
	OutputDevice
    )
    
    
add_executable(compareStorageFiles
    compareStorageFiles.cpp
    )
    
target_link_libraries(compareStorageFiles
    ${OPENSIM_LIBRARIES}
    )
 
    

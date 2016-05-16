include_directories(
    ${OPENSIM_INCLUDE_DIRS}
    ${SIMTK_INCLUDE_DIRS}
    ${rtosim_Utilities_INCLUDE_DIRS}
    ${rtosim_Common_INCLUDE_DIRS}
    ${rtosim_Concurrency_INCLUDE_DIRS}
    ${rtosim_InputDevice_INCLUDE_DIRS}
    ${rtosim_InverseKinematics_INCLUDE_DIRS}
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
    )
    

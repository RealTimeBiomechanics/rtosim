# List source files
set(rtosim_HEADERS  
	rtosim/rtosim.h
    )

source_group("Headers" FILES ${rtosim_HEADERS})
# Define target
add_library(rtosim STATIC ${rtosim_HEADERS})
set_target_properties(rtosim PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(rtosim_common LINK_PRIVATE${OPENSIM_LIBRARIES})
target_include_directories(rtosim PRIVATE 
                                ${CMAKE_CURRENT_SOURCE_DIR} 
                                ${rtosim_Common_INCLUDE_DIRS}
                                ${rtosim_Concurrency_INCLUDE_DIRS} 
                                ${rtosim_InputDevice_INCLUDE_DIRS} 
                                ${rtosim_OutputDevice_INCLUDE_DIRS} 
                                ${rtosim_InverseKinematics_INCLUDE_DIRS}
                                ${rtosim_InverseDynamics_INCLUDE_DIRS}
                                ${rtosim_SignalProcessing_INCLUDE_DIRS}
                                ${rtosim_Utilities_INCLUDE_DIRS} 
                                ${SIMTK_INCLUDE_DIRS} 
                                ${OPENSIM_INCLUDE_DIRS})
# Save path to include directory for "internal" usage
set(rtosim_INCLUDE_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR}     
	${rtosim_Utilities_INCLUDE_DIRS}
    ${rtosim_Common_INCLUDE_DIRS}
    ${rtosim_Concurrency_INCLUDE_DIRS}
    ${rtosim_InputDevice_INCLUDE_DIRS}
	${rtosim_OutputDevice_INCLUDE_DIRS}
    ${rtosim_InverseKinematics_INCLUDE_DIRS}
	${rtosim_InverseDynamics_INCLUDE_DIRS}    
	${rtosim_SignalProcessing_INCLUDE_DIRS} CACHE INTERNAL "")

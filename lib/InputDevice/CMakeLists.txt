set(InputDevice_SOURCES 
#    DataFromNexus.cpp 
    MarkersFromTrc.cpp)

set(InputDevice_HEADERS 
 #   vosl/DataFromNexus.h 
    rtosim/MarkersFromTrc.h
	rtosim/InputDevice.h)

source_group("Headers" FILES ${InputDevice_HEADERS})
source_group("Sources" FILES ${InputDevice_SOURCES})
#link_directories(${ViconSDK_LINK_DIR})
# Define target
add_library(InputDevice STATIC
    ${InputDevice_SOURCES}
    ${InputDevice_HEADERS}
)

target_include_directories(InputDevice PRIVATE
                                 ${rtosim_Concurrency_INCLUDE_DIRS}
								 ${rtosim_Common_INCLUDE_DIRS}
								 ${rtosim_Utilities_INCLUDE_DIRS}
								
                                 # ${CEINMS_INCLUDE_DIRS}
								  #${SharedData_INCLUDE_DIRS}
                                  
                                  ${CMAKE_CURRENT_SOURCE_DIR}
                                  PUBLIC
                                  ${SIMTK_INCLUDE_DIRS}
                                 # ${ViconSDK_INCLUDE_DIR}
                                  ${OPENSIM_INCLUDE_DIRS}
)
TARGET_LINK_LIBRARIES(InputDevice Common Utilities)#SharedData ${ViconSDK_LIBRARIES})
# Save path to include directory for "internal" usage
set(rtosim_InputDevice_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for InputDevice library")
# Install

#install(FILES ${InputDevice_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl)
#install(TARGETS InputDevice
#        EXPORT VOSLTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
 #       )

set(OutputDevice_SOURCES 
	StateVisualiser.cpp
    )

set(OutputDevice_HEADERS 
    rtosim/QueueToFileLogger.h
	rtosim/OutputDevice.h
    rtosim/FileLogger.h
    rtosim/FrameCounter.h
	rtosim/StateVisualiser.h
)

set(OutputDevice_TEMPLATE_IMPLEMENTATIONS 
    FileLogger.cpp
	QueueToFileLogger.cpp
    FrameCounter.cpp
)    
    
set_source_files_properties(${OutputDevice_TEMPLATE_IMPLEMENTATIONS} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("Headers" FILES ${OutputDevice_HEADERS})
source_group("Sources" FILES ${OutputDevice_SOURCES} ${OutputDevice_TEMPLATE_IMPLEMENTATIONS})

add_library(OutputDevice STATIC
    ${OutputDevice_SOURCES}
    ${OutputDevice_HEADERS}
	${OutputDevice_TEMPLATE_IMPLEMENTATIONS}
)

target_include_directories(
    OutputDevice 
    PRIVATE
        ${rtosim_Concurrency_INCLUDE_DIRS}
        ${rtosim_Common_INCLUDE_DIRS}
        ${rtosim_Utilities_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
        ${SIMTK_INCLUDE_DIRS}
        ${OPENSIM_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(OutputDevice )#SharedData ${ViconSDK_LIBRARIES})
# Save path to include directory for "internal" usage
set(rtosim_OutputDevice_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for OutputDevice library")
# Install

#install(FILES ${OutputDevice_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl)
#install(TARGETS OutputDevice
#        EXPORT VOSLTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
 #       )

# List source files
set(Common_HEADERS  
	rtosim/QueueData.h
    rtosim/MarkerData.h
    rtosim/GeneralisedCoordinatesData.h
    rtosim/EndOfData.h
    rtosim/QueuesSync.h
	rtosim/FlowControl.h
	rtosim/Common.h
    rtosim/ExternalForceData.h
    rtosim/ExternalLoadProperties.h
    rtosim/ExternalForceProperties.h
    )

set(Common_QUEUES_HEADERS 
    rtosim/queue/MarkerSetQueue.h
    rtosim/queue/GeneralisedCoordinatesQueue.h
    rtosim/queue/MultipleExternalForcesQueue.h
    rtosim/queue/ExternalTorquesQueue.h
	rtosim/queue/ScalarVectorQueue.h
	rtosim/queue/SingleExternalForceQueue.h
	) 
    
set(Common_SOURCES 
	MarkerData.cpp
    GeneralisedCoordinatesData.cpp
    ExternalForceData.cpp
	FlowControl.cpp
    ExternalLoadProperties.cpp
    ExternalForceProperties.cpp
    )

set(Common_TEMPLATE_IMPLEMENTATION	)


set_source_files_properties( ${Common_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${Common_HEADERS} ${Common_QUEUES_HEADERS})
source_group("Sources" FILES ${Common_SOURCES} ${Common_TEMPLATE_IMPLEMENTATION} )

# Define target
add_library(Common STATIC ${Common_SOURCES} ${Common_HEADERS} ${Common_QUEUES_HEADERS} ${Common_TEMPLATE_IMPLEMENTATION})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(rtosim_common LINK_PRIVATE${OPENSIM_LIBRARIES})
target_include_directories(Common  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${Concurrency_INCLUDE_DIRS} ${SIMTK_INCLUDE_DIRS} ${OPENSIM_INCLUDE_DIRS})

# Save path to include directory for "internal" usage
set(rtosim_Common_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for library of common files")

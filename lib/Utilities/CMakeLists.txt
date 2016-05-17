# List source files
set(Utilities_HEADERS  
	rtosim/Utilities.h
    rtosim/ProgramOptionsParser.h
    rtosim/Mapper.h
    rtosim/ArrayConverter.h
    rtosim/StopWatch.h
    rtosim/FileSystem.h
    rtosim/OsimUtilities.h
    )

set(Utilities_SOURCES 
    ProgramOptionsParser.cpp
    Mapper.cpp
    StopWatch.cpp
    FileSystem.cpp
    OsimUtilities.cpp
    )

set(Utilities_TEMPLATE_IMPLEMENTATION	)


set_source_files_properties( ${Utilities_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${Utilities_HEADERS})
source_group("Sources" FILES ${Utilities_SOURCES} ${Utilities_TEMPLATE_IMPLEMENTATION} )

# Define target
add_library(Utilities STATIC ${Utilities_SOURCES} ${Utilities_HEADERS} ${Utilities_TEMPLATE_IMPLEMENTATION})
target_link_libraries(Utilities LINK_PUBLIC ${OPENSIM_LIBRARIES})
target_include_directories(Utilities PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${SIMTK_INCLUDE_DIRS} ${OPENSIM_INCLUDE_DIRS})

# Save path to include directory for "internal" usage
set(rtosim_Utilities_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for SignalProcessing files")

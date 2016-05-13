# List source files
set(Utilities_HEADERS  
    rtosim/ProgramOptionsParser.h
    rtosim/Mapper.h
    rtosim/ArrayConverter.h
    )

set(Utilities_SOURCES 
    ProgramOptionsParser.cpp
    Mapper.cpp)

set(Utilities_TEMPLATE_IMPLEMENTATION	)


set_source_files_properties( ${Utilities_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${Utilities_HEADERS})
source_group("Sources" FILES ${Utilities_SOURCES} ${Utilities_TEMPLATE_IMPLEMENTATION} )

# Define target
add_library(Utilities STATIC ${Utilities_SOURCES} ${Utilities_HEADERS} ${Utilities_TEMPLATE_IMPLEMENTATION})
#target_link_libraries(rtosim_common LINK_PRIVATE${OPENSIM_LIBRARIES})
#target_include_directories(ceinms_common PUBLIC ${Boost_INCLUDE_DIRS} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${SIMTK_INCLUDE_DIRS} ${OPENSIM_INCLUDE_DIRS})

# Save path to include directory for "internal" usage
set(rtosim_Utilities_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for library of common files")

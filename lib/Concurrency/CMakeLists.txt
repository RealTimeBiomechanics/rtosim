#Author: Elena Ceseracciu

set(Concurrency_HEADERS rtosim/concurrency/Latch.h
                        rtosim/concurrency/Queue.h)

set(Concurrency_TEMPLATE_IMPLEMENTATIONS rtosim/concurrency/Queue.cpp)
set_source_files_properties(${Concurrency_TEMPLATE_IMPLEMENTATIONS} PROPERTIES HEADER_FILE_ONLY TRUE)

set(Concurrency_SOURCES Latch.cpp)

source_group("Header files" FILES ${Concurrency_HEADERS})
source_group("Source files" FILES ${Concurrency_TEMPLATE_IMPLEMENTATIONS} ${Concurrency_SOURCES})

include_directories(include)
add_library(Concurrency ${Concurrency_HEADERS} ${Concurrency_TEMPLATE_IMPLEMENTATIONS} ${Concurrency_SOURCES} )

set(rtosim_Concurrency_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for Concurrency library")

#install(FILES ${Concurrency_HEADERS} ${Concurrency_TEMPLATE_IMPLEMENTATIONS}
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rtosim/concurrency COMPONENT Development)

#install(TARGETS Concurrency
#        EXPORT CEINMSTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Applications
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)


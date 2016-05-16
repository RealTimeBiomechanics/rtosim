## Authors: Monica Reggiani, Claudio Pizzolato, Elena Ceseracciu

# List source files
set(InverseKinematics_SOURCES
                        IKSequencer.cpp 
						IKSolverParallel.cpp 
                        JobsCreator.cpp 
						MarkersReferenceFromQueue.cpp 
                        IKFromQueue.cpp
						TimeSequence.cpp
						)

set(InverseKinematics_HEADERS
						rtosim/InverseKinematics.h
                        rtosim/IKSequencer.h 
						rtosim/IKSolverParallel.h 
						rtosim/IKoutputs.h
                        rtosim/JobsCreator.h 
						rtosim/MarkersReferenceFromQueue.h 
						rtosim/ThreadPoolJobs.h
                        rtosim/IKFromQueue.h
                        rtosim/TimeSequence.h
						)

set(InverseKinematics_TEMPLATE_IMPLEMENTATION IKoutputs.cpp ThreadPoolJobs.cpp)


set_source_files_properties( ${InverseKinematics_TEMPLATE_IMPLEMENTATION} PROPERTIES HEADER_FILE_ONLY TRUE)
source_group("Headers" FILES ${InverseKinematics_HEADERS})
source_group("Sources" FILES ${InverseKinematics_SOURCES}  ${InverseKinematics_TEMPLATE_IMPLEMENTATION})

# Define target
add_library(InverseKinematics STATIC
  ${InverseKinematics_SOURCES}
  ${InverseKinematics_HEADERS}
  ${InverseKinematics_TEMPLATE_IMPLEMENTATION}
)

target_include_directories(InverseKinematics PRIVATE
									         ${rtosim_Common_INCLUDE_DIRS}
								    	     ${rtosim_Concurrency_INCLUDE_DIRS}
										     ${rtosim_Utilities_INCLUDE_DIRS}
								    	     ${CMAKE_CURRENT_SOURCE_DIR}
                                             PUBLIC
                                             ${OPENSIM_INCLUDE_DIRS}
                                             ${SIMTK_INCLUDE_DIRS}
                                    )

# Save path to include directory for "internal" usage
set(rtosim_InverseKinematics_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Include directory for InverseKinematics library")
# Install

#install(FILES ${InverseKinematics_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vosl)
#install(TARGETS InverseKinematics
#        EXPORT VOSLTargets
#        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#        #INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}# only available from CMake 2.8.12
#        )

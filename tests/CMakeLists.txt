include_directories(
    ${OPENSIM_INCLUDE_DIRS}
    ${SIMTK_INCLUDE_DIRS}
    ${rtosim_Utilities_INCLUDE_DIRS}
    )
    

add_executable(TestDependencies 
    testDependencies.cpp
    )

target_link_libraries(TestDependencies 
    ${OPENSIM_LIBRARIES}
    ${SIMTK_LIBRARIES}
    )
    
add_executable(TestFileSystem
    testFileSystem.cpp
    )
    
target_link_libraries(TestFileSystem 
	${SIMTK_LIBRARIES}
	Utilities
    )

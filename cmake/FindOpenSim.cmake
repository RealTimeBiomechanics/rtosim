set(OPENSIM_POSSIBLE_DIRECTORIES 
	$ENV{OPENSIM_HOME}
	)

find_path(OPENSIM_INCLUDE_DIRS OpenSim/OpenSim.h
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES sdk/include )
MESSAGE( STATUS "OpenSim Include path: ${OPENSIM_INCLUDE_DIRS}")

find_path(SIMTK_INCLUDE_DIRS Simbody.h
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES sdk/include/SimTK/include sdk/include/SimTK/simbody)
MESSAGE( STATUS "SimTK Include path: ${SIMTK_INCLUDE_DIRS}")


if(UNIX)
    set(NameSpace )
else()
    set(NameSpace OpenSim_)
endif()

find_library(osimSimulation
             NAMES osimSimulation
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimCommon
             NAMES osimCommon
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimActuators
             NAMES osimActuators
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimAnalyses
             NAMES osimAnalyses
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimTools
             NAMES osimTools
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(${NameSpace}SimTKcommon
             NAMES ${NameSpace}SimTKcommon SimTKcommon
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(${NameSpace}SimTKmath
             NAMES ${NameSpace}SimTKmath SimTKmath
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(${NameSpace}SimTKsimbody SimTKsimbody
             NAMES ${NameSpace}SimTKsimbody
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(SimTKlapack
             NAMES SimTKlapack lapack liblapack
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

set(OPENSIM_LIBRARIES 
    ${osimSimulation} 
    ${osimCommon} 
    ${osimActuators} 
    ${osimAnalyses} 
    ${osimTools} 
    ${${NameSpace}SimTKcommon} 
    ${${NameSpace}SimTKmath} 
    ${${NameSpace}SimTKsimbody} 
    ${${NameSpace}SimTKlapack} 
    ${SimTKlapack})


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set OPENSIM_FOUND to TRUE
# if all listed variables are TRUE
# DEFAULT_MSG is predefined... change only if you need a custom msg
find_package_handle_standard_args(OpenSim DEFAULT_MSG
                                  OPENSIM_LIBRARIES OPENSIM_INCLUDE_DIRS SIMTK_INCLUDE_DIRS)



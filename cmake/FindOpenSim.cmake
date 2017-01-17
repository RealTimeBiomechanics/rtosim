# - Try to find OpenSim and SimTK
# Once done this will define
#  OPENSIM_FOUND - System has OpenSim
#  OPENSIM_INCLUDE_DIRS - The OpenSim include directories
#  SIMTK_INCLUDE_DIRS - The SimTK include directories
#  OPENSIM_LIBRARIES - The libraries needed to use Opensim
#  OPENSIM_DEFINITIONS - Compiler switches required for using OpenSim


set(OPENSIM_POSSIBLE_DIRECTORIES 
	$ENV{OPENSIM_HOME}
	)

find_path(OPENSIM_INCLUDE_DIRS OpenSim/OpenSim.h
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES sdk/include )
MESSAGE( STATUS "OpenSim Include path: ${OPENSIM_INCLUDE_DIRS}")

find_path(SIMTK_INCLUDE_DIRS Simbody.h
          PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES sdk/include/SimTK/include)
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

find_library(osimSimulation_d
             NAMES osimSimulation_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)
             
             
find_library(osimCommon
             NAMES osimCommon
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimCommon_d
             NAMES osimCommon_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)
             
find_library(osimActuators
             NAMES osimActuators
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimActuators_d
             NAMES osimActuators_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

             
find_library(osimAnalyses
             NAMES osimAnalyses
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimAnalyses_d
             NAMES osimAnalyses_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

             
find_library(osimTools
             NAMES osimTools
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(osimTools_d
             NAMES osimTools_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

             
find_library(${NameSpace}SimTKcommon
             NAMES ${NameSpace}SimTKcommon SimTKcommon
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(${NameSpace}SimTKcommon_d
            NAMES ${NameSpace}SimTKcommon_d SimTKcommon_d
            PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
            PATH_SUFFIXES sdk/lib lib)

             
find_library(${NameSpace}SimTKmath
             NAMES ${NameSpace}SimTKmath SimTKmath
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(${NameSpace}SimTKmath_d
             NAMES ${NameSpace}SimTKmath_d SimTKmath_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

             
find_library(${NameSpace}SimTKsimbody SimTKsimbody
             NAMES ${NameSpace}SimTKsimbody
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

find_library(${NameSpace}SimTKsimbody_d SimTKsimbody_d
             NAMES ${NameSpace}SimTKsimbody_d
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)

             #does this exist?
find_library(SimTKlapack
             NAMES SimTKlapack lapack liblapack
             PATHS ${OPENSIM_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES sdk/lib lib)


             
set(OPENSIM_LIBRARIES 
	optimized ${osimSimulation} 
	optimized ${osimCommon} 
	optimized ${osimActuators}
	optimized ${osimAnalyses} 
	optimized ${osimTools} 
#	optimized ${${NameSpace}SimTKcommon} 
#	optimized ${${NameSpace}SimTKmath} 
#	optimized ${${NameSpace}SimTKsimbody} 
#	optimized ${${NameSpace}SimTKlapack} 
	optimized ${SimTKlapack} 
	debug ${osimSimulation_d} 
	debug ${osimCommon_d} 
	debug ${osimActuators_d} 
	debug ${osimAnalyses_d} 
	debug ${osimTools_d} 
#	debug ${${NameSpace}SimTKcommon_d} 
#	debug ${${NameSpace}SimTKmath_d} 
#	debug ${${NameSpace}SimTKsimbody_d} 
#	debug ${${NameSpace}SimTKlapack} 
	debug ${SimTKlapack})


#IF(WIN32)
#	SET(PLATFORM_LIBS  pthreadVC2)
#ELSE (WIN32)
#  SET(NameSpace "")
#  IF(APPLE)
#	SET(PLATFORM_LIBS  SimTKAtlas)
#  ELSE(APPLE)
#	SET(PLATFORM_LIBS SimTKAtlas_Lin_generic)
#  ENDIF(APPLE)
#ENDIF (WIN32)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set OPENSIM_FOUND to TRUE
# if all listed variables are TRUE
# DEFAULT_MSG is predefined... change only if you need a custom msg
find_package_handle_standard_args(OpenSim DEFAULT_MSG
                                  OPENSIM_LIBRARIES OPENSIM_INCLUDE_DIRS SIMTK_INCLUDE_DIRS)

# Mark the following variables as advanced (they are not displayed in any 
# of the cmake GUIs unless the show advanced option is on.
#mark_as_advanced(LIBXML2_INCLUDE_DIR LIBXML2_LIBRARY )



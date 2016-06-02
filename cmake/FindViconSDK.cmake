set(ViconSDK_POSSIBLE_DIRECTORIES 
	$ENV{VICONDATASTREAM_INSTALL_DIR}
	)

find_path(ViconSDK_INCLUDE_DIRS Client.h
          PATHS ${ViconSDK_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES include )
MESSAGE( STATUS "ViconSDK Include path: ${ViconSDK_INCLUDE_DIRS}")

find_library(ViconDataStreamSDK_CPP
             NAMES ViconDataStreamSDK_CPP
             PATHS ${ViconSDK_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES lib)
             
set(ViconSDK_LIBRARIES 
    ${ViconDataStreamSDK_CPP} )
    
MESSAGE( STATUS "ViconSDK libraries: ${ViconSDK_LIBRARIES}")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set OPENSIM_FOUND to TRUE
# if all listed variables are TRUE
# DEFAULT_MSG is predefined... change only if you need a custom msg
find_package_handle_standard_args(ViconSDK DEFAULT_MSG
                                  ViconSDK_INCLUDE_DIRS ViconSDK_LIBRARIES)



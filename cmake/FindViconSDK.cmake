#-------------------------------------------------------------------------- #
# Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                         #
#                                                                           #
# Licensed under the Apache License, Version 2.0 (the "License");           #
# you may not use this file except in compliance with the License.          #
# You may obtain a copy of the License at:                                  #
# http://www.apache.org/licenses/LICENSE-2.0                                #
#                                                                           #
# Unless required by applicable law or agreed to in writing, software       #
# distributed under the License is distributed on an "AS IS" BASIS,         #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  #
# See the License for the specific language governing permissions and       #
# limitations under the License.                                            #
# --------------------------------------------------------------------------#

set(ViconSDK_POSSIBLE_DIRECTORIES 
	$ENV{VICONDATASTREAM_DIR}
	)

find_path(ViconSDK_INCLUDE_DIRS DataStreamClient.h
          PATHS ${ViconSDK_POSSIBLE_DIRECTORIES}
          PATH_SUFFIXES Win64/CPP CPP include )
MESSAGE( STATUS "ViconSDK Include path: ${ViconSDK_INCLUDE_DIRS}")

find_library(ViconDataStreamSDK_CPP
             NAMES ViconDataStreamSDK_CPP
             PATHS ${ViconSDK_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES Win64/CPP CPP lib)
             
set(ViconSDK_LIBRARIES 
    ${ViconDataStreamSDK_CPP} )
    
MESSAGE( STATUS "ViconSDK libraries: ${ViconSDK_LIBRARIES}")
 
 
# handle the QUIETLY and REQUIRED arguments and set VICONSDK_FOUND to TRUE
# if all listed variables are TRUE
# DEFAULT_MSG is predefined... change only if you need a custom msg
find_package_handle_standard_args(ViconSDK DEFAULT_MSG
                                  ViconSDK_INCLUDE_DIRS ViconSDK_LIBRARIES)



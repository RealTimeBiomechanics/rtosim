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

set(Osens_POSSIBLE_DIRECTORIES 
	~/testlibs
	)

find_path(Osens_INCLUDE_DIRS OpenSim/Common/OrientationSensorData.h
          PATHS ${Osens_POSSIBLE_DIRECTORIES})


MESSAGE( STATUS "Osens Include path: ${Osens_INCLUDE_DIRS}")

find_library(IKextended
             NAMES IKextended
             PATHS ${Osens_POSSIBLE_DIRECTORIES}
             PATH_SUFFIXES lib)
             
set(Osens_LIBRARIES 
    ${IKextended} )
    
MESSAGE( STATUS "Osens libraries: ${Osens_LIBRARIES}")
 
 
# handle the QUIETLY and REQUIRED arguments and set OSENS_FOUND to TRUE
# if all listed variables are TRUE
# DEFAULT_MSG is predefined... change only if you need a custom msg
find_package_handle_standard_args(Osens DEFAULT_MSG
                                  Osens_INCLUDE_DIRS Osens_LIBRARIES)



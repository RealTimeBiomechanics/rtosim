/* -------------------------------------------------------------------------- *
 * Copyright (c) 2010-2016 C. Pizzolato, M. Reggiani                          *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at:                                   *
 * http://www.apache.org/licenses/LICENSE-2.0                                 *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

#ifndef rtosim_InputDevice_h
#define rtosim_InputDevice_h

#include "rtosim/MarkersFromTrc.h"
#include "rtosim/ExternalForcesFromStorageFile.h"
#include "rtosim/GeneralisedCoordinatesFromStorageFile.h"
#include "rtosim/OrientationsFromMot.h"
#include "rtosim/OrientationsFromMobile.h"
#ifdef INCLUDEVICONSDK
	#include "rtosim/DataFromNexus.h"
#endif
#ifdef INCLUDEMR3TOOLS
	#include "rtosim/OrientationsFromMR3.h"
#endif
#endif
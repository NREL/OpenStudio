/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_CORE_DEPRECATED_HPP
#define UTILITIES_CORE_DEPRECATED_HPP

/****************************************************************************
!!! THIS FILE MUST BE INCLUDED BY ANY SOURCE FILE THAT USES OPENSTUDIO_DEPRECATED!!!
*****************************************************************************/

#include <utilities/UtilitiesAPI.hpp>

#ifdef __GNUC__
  #define OS_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
  #define OS_DEPRECATED __declspec(deprecated)
#else
  #pragma message("WARNING: You need to implement DEPRECATED for this compiler")
  #define OS_DEPRECATED
#endif

#endif // UTILITIES_CORE_DEPRECATED_HPP

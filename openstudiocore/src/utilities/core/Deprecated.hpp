/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "../UtilitiesAPI.hpp"

#ifdef __GNUC__
  #define OS_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
  /// In MSVC this will generate warning C4996
  /// To intentionally disable this warning, e.g. in test code that still uses deprecated functionality
  /// place this around the code that uses the deprecated functionality
  /// #if defined(_MSC_VER)
  ///   #pragma warning( push )
  ///   #pragma warning( disable : 4996 )
  /// #endif
  /// Some code
  /// #if defined(_MSC_VER)
  ///   #pragma warning( pop ) 
  /// #endif

  #define OS_DEPRECATED __declspec(deprecated)
#else
  #pragma message("WARNING: You need to implement DEPRECATED for this compiler")
  #define OS_DEPRECATED
#endif

#endif // UTILITIES_CORE_DEPRECATED_HPP

/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_DEPRECATED_HPP
#define UTILITIES_CORE_DEPRECATED_HPP

/****************************************************************************
!!! THIS FILE MUST BE INCLUDED BY ANY SOURCE FILE THAT USES OPENSTUDIO_DEPRECATED!!!
*****************************************************************************/

#include "../UtilitiesAPI.hpp"

#ifdef __GNUC__
#  define OS_DEPRECATED __attribute__((deprecated))
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

#  define OS_DEPRECATED __declspec(deprecated)
#else
#  pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#  define OS_DEPRECATED
#endif

#endif  // UTILITIES_CORE_DEPRECATED_HPP

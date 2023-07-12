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

#define OS_DEPRECATED(__deprecatedAtVersionMajor__, __deprecatedAtVersionMinor__, __deprecatedAtVersionPatch__)                   \
  [[deprecated("Deprecated at " #__deprecatedAtVersionMajor__ "." #__deprecatedAtVersionMinor__ "." #__deprecatedAtVersionPatch__ \
               ". It will be removed after three releases.")]]
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

#endif  // UTILITIES_CORE_DEPRECATED_HPP

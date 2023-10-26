/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_SYSTEM_HPP
#define UTILITIES_CORE_SYSTEM_HPP

#include "../UtilitiesAPI.hpp"

namespace openstudio {

class UTILITIES_API System
{
 public:
  /// Sleep for number of milliseconds
  static void msleep(int msecs);

  /// Returns the number of processors on this computer
  static unsigned numberOfProcessors();

  /// \note not using string_view because we need null terminated strings
  /// Utility for testing exception handling within the system
  static void testExceptions1();
  static void testExceptions2();
  static void testExceptions3();
  static void testExceptions4();
  static void testExceptions5();
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_SYSTEM_HPP

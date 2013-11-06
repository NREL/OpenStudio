/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef UTILITIES_CORE_SYSTEM_HPP
#define UTILITIES_CORE_SYSTEM_HPP

#include <utilities/UtilitiesAPI.hpp>
#include <utilities/time/Time.hpp>

#include <boost/optional.hpp>

namespace openstudio {

  class UTILITIES_API System{
  public:

    /// Returns the time that the system has been idle
    static boost::optional<Time> systemIdleTime();

    /// Sleep for number of milliseconds
    static void msleep(int msecs); 

    /// Returns the number of processors on this computer
    static unsigned numberOfProcessors();

    /// Utility for testing exception handling within the system
    static void testExceptions1();
    static void testExceptions2();
    static void testExceptions3();
    static void testExceptions4();
    static void testExceptions5();
  };

} // openstudio

#endif // UTILITIES_CORE_SYSTEM_HPP

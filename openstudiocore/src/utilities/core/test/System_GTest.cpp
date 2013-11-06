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

#include <gtest/gtest.h>

#include <utilities/core/System.hpp>

using openstudio::System;
using openstudio::Time;

TEST(System, IdleTime)
{
  // system idle time should not be unset for platforms it is implemented on
  boost::optional<Time> idleTime = System::systemIdleTime();

  #ifdef _WINDOWS
    EXPECT_TRUE(idleTime);
  #else

  #endif
}

TEST(System, ExceptionHandling)
{
  System::testExceptions1();
  System::testExceptions2();
  System::testExceptions3();
  System::testExceptions4();
  System::testExceptions5();

}

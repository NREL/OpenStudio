/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../System.hpp"

using openstudio::System;

TEST(System, ExceptionHandling) {
  System::testExceptions1();
  System::testExceptions2();
  System::testExceptions3();
  System::testExceptions4();
  System::testExceptions5();
}

TEST(System, MSleep) {
  // make sure this doesn't timeout
  System::msleep(10);
}

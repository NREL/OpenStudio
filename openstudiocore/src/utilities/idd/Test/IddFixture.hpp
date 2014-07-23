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

#ifndef UTILITIES_IDD_TEST_IDDFIXTURE_HPP
#define UTILITIES_IDD_TEST_IDDFIXTURE_HPP

#include "../IddFile.hpp"
#include <utilities/idd/IddFactory.hxx>
#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"
#include "../../time/Time.hpp"

#include <resources.hxx>

#include <fstream>

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

class IddFixture : public ::testing::Test {
protected:

  // initialize for each test
  virtual void SetUp() {}

  // tear down after for each test
  virtual void TearDown() {}

  // initialize static members
  static void SetUpTestCase();

  // tear down static members
  static void TearDownTestCase();

public:

  REGISTER_LOGGER("IddFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static openstudio::IddFile epIddFile;
  static openstudio::IddFile osIddFile;
  static openstudio::Time iddLoadTime;
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif // UTILITIES_IDD_TEST_IDDFIXTURE_HPP

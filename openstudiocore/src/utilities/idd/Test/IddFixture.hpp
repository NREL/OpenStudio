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

#ifndef ENERGYPLUS_IDD_TEST_IDDTESTFIXTURE_HPP
#define ENERGYPLUS_IDD_TEST_IDDTESTFIXTURE_HPP

#include <utilities/idd/IddFile.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/core/Logger.hpp>
#include <utilities/core/FileLogSink.hpp>
#include <utilities/time/Time.hpp>

#include <resources.hxx>

#include <fstream>

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>

class IddFixture : public ::testing::Test {
protected:

  // initiallize for each test
  virtual void SetUp() {}

  // tear down after for each test
  virtual void TearDown() {}

  // initiallize static members
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

#endif //ENERGYPLUS_IDD_TEST_IDDTESTFIXTURE_HPP

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

#ifndef UTILITIES_SQL_TEST_SQLFILELARGEFIXTURE_HPP
#define UTILITIES_SQL_TEST_SQLFILELARGEFIXTURE_HPP

#include <gtest/gtest.h>

#include "../SqlFile.hpp"
#include "../../core/Path.hpp"

#include <resources.hxx>

class SqlFileLargeFixture : public ::testing::Test {
protected:

  // initialize for each test
  virtual void SetUp();

  // tear down after for each test
  virtual void TearDown();

  // initialize static members
  static void SetUpTestCase();

  // tear down static members
  static void TearDownTestCase();

  // set up logging
  REGISTER_LOGGER("SqlFileLargeFixture");

public:
  // sql files
  static openstudio::SqlFile sqlFile;
  static std::shared_ptr<openstudio::TextLogType> logFile;
};

#endif // UTILITIES_SQL_TEST_SQLFILELARGEFIXTURE_HPP


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

#ifndef MODEL_TEST_MODELFIXTURE_HPP
#define MODEL_TEST_MODELFIXTURE_HPP

#include <gtest/gtest.h>

#include <resources.hxx>

#include "../Model.hpp"

#include "../../utilities/idd/IddField.hpp"
#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

class ModelFixture : public ::testing::Test {
 protected:
  // initialize for each test
  virtual void SetUp();

  // tear down after each test
  virtual void TearDown();

  // initialize static members
  static void SetUpTestCase();

  // tear down static members
  static void TearDownTestCase();

  // test that the given iddField is a boolean Yes/No choice field
  void testBooleanIddField(const openstudio::IddField& iddField,
                           const boost::optional<std::string>& defaultValue);

  // set up logging
  REGISTER_LOGGER("ModelFixture");

  // Note: storage for static variables must be defined in a separate .cpp file
  static double tol;
  static boost::optional<openstudio::FileLogSink> logFile;
};

#endif // MODEL_TEST_MODELFIXTURE_HPP

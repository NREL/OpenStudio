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

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"
#include "../ResultsTranslator.hpp"

#include "../../model/Model.hpp"

#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include <resources.hxx>

#include <sstream>

using openstudio::model::OptionalModel;
using openstudio::energyplus::ForwardTranslator;
using openstudio::energyplus::ReverseTranslator;
using openstudio::energyplus::ResultsTranslator;
using openstudio::energyplus::ErrorFile;
using openstudio::IdfFile;
using openstudio::Workspace;
using openstudio::OptionalWorkspace;
using openstudio::SqlFile;
using openstudio::OptionalInt;

// DLM@20101005: deprecated as of E+ 6
/*
TEST_F(EnergyPlusFixture,ResultsTranslator_5ZoneAirCooled_NoErrorsNoWarnings)
{
  openstudio::path epDir = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/");
  openstudio::path origSqlPath = epDir / openstudio::toPath("eplusout.sql");
  openstudio::path testSqlPath = epDir / openstudio::toPath("eplusout_NoErrorsNoWarnings.sql");
  boost::filesystem::copy_file(origSqlPath,testSqlPath,boost::filesystem::copy_option::overwrite_if_exists);
  openstudio::path errorPath = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/NoErrorsNoWarnings.err");

  SqlFile sqlFile(testSqlPath);
  ErrorFile errorFile(errorPath);

  ASSERT_TRUE(sqlFile.connectionOpen());

  ResultsTranslator resultsTranslator(sqlFile);

  // import error file content
  ASSERT_TRUE(resultsTranslator.importErrorFile(errorFile));

  ASSERT_TRUE(sqlFile.connectionOpen());

  OptionalInt completed = sqlFile.execAndReturnFirstInt("SELECT Completed FROM simulations");
  ASSERT_TRUE(completed);
  EXPECT_TRUE(errorFile.completed());
  EXPECT_EQ(1, *completed);

  OptionalInt completedSuccessfully = sqlFile.execAndReturnFirstInt("SELECT CompletedSuccessfully FROM simulations");
  ASSERT_TRUE(completedSuccessfully);
  EXPECT_TRUE(errorFile.completedSuccessfully());
  EXPECT_EQ(1, *completedSuccessfully);
}

TEST_F(EnergyPlusFixture,ResultsTranslator_5ZoneAirCooled_WarningsAndSevere)
{
  openstudio::path epDir = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/");
  openstudio::path origSqlPath = epDir / openstudio::toPath("eplusout.sql");
  openstudio::path testSqlPath = epDir / openstudio::toPath("eplusout_WarningsAndSevere.sql");
  boost::filesystem::copy_file(origSqlPath, testSqlPath, boost::filesystem::copy_option::overwrite_if_exists);
  openstudio::path errorPath = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/WarningsAndSevere.err");

  SqlFile sqlFile(testSqlPath);
  ErrorFile errorFile(errorPath);

  ASSERT_TRUE(sqlFile.connectionOpen());

  ResultsTranslator resultsTranslator(sqlFile);

  // import error file content
  ASSERT_TRUE(resultsTranslator.importErrorFile(errorFile));

  ASSERT_TRUE(sqlFile.connectionOpen());

  OptionalInt completed = sqlFile.execAndReturnFirstInt("SELECT Completed FROM simulations");
  ASSERT_TRUE(completed);
  EXPECT_TRUE(errorFile.completed());
  EXPECT_EQ(1, *completed);

  OptionalInt completedSuccessfully = sqlFile.execAndReturnFirstInt("SELECT CompletedSuccessfully FROM simulations");
  ASSERT_TRUE(completedSuccessfully);
  EXPECT_FALSE(errorFile.completedSuccessfully());
  EXPECT_EQ(0, *completedSuccessfully);
}

TEST_F(EnergyPlusFixture,ResultsTranslator_5ZoneAirCooled_WarningsAndCrash)
{
  openstudio::path epDir = resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/");
  openstudio::path origSqlPath = epDir / openstudio::toPath("eplusout.sql");
  openstudio::path testSqlPath = epDir / openstudio::toPath("eplusout_WarningsAndCrash.sql");
  boost::filesystem::copy_file(origSqlPath,testSqlPath,boost::filesystem::copy_option::overwrite_if_exists);
  openstudio::path errorPath = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/WarningsAndCrash.err");

  SqlFile sqlFile(testSqlPath);
  ErrorFile errorFile(errorPath);

  ASSERT_TRUE(sqlFile.connectionOpen());

  ResultsTranslator resultsTranslator(sqlFile);

  // import error file content
  ASSERT_TRUE(resultsTranslator.importErrorFile(errorFile));

  ASSERT_TRUE(sqlFile.connectionOpen());

  OptionalInt completed = sqlFile.execAndReturnFirstInt("SELECT Completed FROM simulations");
  ASSERT_TRUE(completed);
  EXPECT_FALSE(errorFile.completed());
  EXPECT_EQ(0, *completed);

  OptionalInt completedSuccessfully = sqlFile.execAndReturnFirstInt("SELECT CompletedSuccessfully FROM simulations");
  ASSERT_TRUE(completedSuccessfully);
  EXPECT_FALSE(errorFile.completedSuccessfully());
  EXPECT_EQ(0, *completedSuccessfully);
}
*/

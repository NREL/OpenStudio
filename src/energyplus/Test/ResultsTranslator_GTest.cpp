/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
  openstudio::filesystem::copy_file(origSqlPath,testSqlPath,openstudio::filesystem::copy_option::overwrite_if_exists);
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
  openstudio::filesystem::copy_file(origSqlPath, testSqlPath, openstudio::filesystem::copy_option::overwrite_if_exists);
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
  openstudio::filesystem::copy_file(origSqlPath,testSqlPath,openstudio::filesystem::copy_option::overwrite_if_exists);
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

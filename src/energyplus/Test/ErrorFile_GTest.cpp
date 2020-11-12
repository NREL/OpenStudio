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

#include "../../utilities/core/Logger.hpp"

#include <resources.hxx>

#include <sstream>
#include <fstream>

using openstudio::energyplus::ErrorFile;

TEST_F(EnergyPlusFixture, ErrorFile_NoErrorsNoWarnings) {
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/NoErrorsNoWarnings.err");

  ErrorFile errorFile(path);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.warnings().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.fatalErrors().size());
  EXPECT_TRUE(errorFile.completed());
  EXPECT_TRUE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture, ErrorFile_Warnings) {
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/Warnings.err");

  ErrorFile errorFile(path);
  ASSERT_EQ(static_cast<unsigned>(1), errorFile.warnings().size());
  EXPECT_EQ(
    "Weather file location will be used rather than entered Location object.\n ..Location object=USA CO-BOULDER\n ..Weather File Location=Denver "
    "Centennial  Golden   Nr CO USA TMY3 WMO#=724666\n ..due to location differences, Latitude difference=[0.28] degrees, Longitude "
    "difference=[7.00E-002] degrees.\n ..Time Zone difference=[0.0] hour(s), Elevation difference=[11.93] percent, [195.00] meters.",
    errorFile.warnings()[0]);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.fatalErrors().size());
  EXPECT_TRUE(errorFile.completed());
  EXPECT_TRUE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture, ErrorFile_SevereErrors) {
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/SevereErrors.err");

  ErrorFile errorFile(path);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.warnings().size());
  ASSERT_EQ(static_cast<unsigned>(26), errorFile.severeErrors().size());
  EXPECT_EQ("Out of range value Numeric Field#1 (Lighting Level), value=-1527598.22490, range={>=0}, in LIGHTS=ZN_1_FLR_1_SEC_1_LIGHTS",
            errorFile.severeErrors()[0]);
  EXPECT_EQ("IP: Out of \"range\" values and/or blank required fields found in input", errorFile.severeErrors()[25]);
  ASSERT_EQ(static_cast<unsigned>(1), errorFile.fatalErrors().size());
  EXPECT_EQ("IP: Errors occurred on processing IDF file. Preceding condition(s) cause termination.", errorFile.fatalErrors()[0]);
  EXPECT_TRUE(errorFile.completed());
  EXPECT_FALSE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture, ErrorFile_WarningsAndSevere) {
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/WarningsAndSevere.err");

  ErrorFile errorFile(path);
  ASSERT_EQ(static_cast<unsigned>(46), errorFile.warnings().size()) << "Number of warnings mistmatch for error file at '" << path << "'.";
  EXPECT_EQ("Output:PreprocessorMessage=\"EPXMLPreProc2\" has the following Warning conditions:\n Requested glazing exceeds available area for\n "
            "B6CCD5_window_1.  Reducing sill height to fit.",
            errorFile.warnings()[0]);
  EXPECT_EQ(static_cast<unsigned>(8), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(1), errorFile.fatalErrors().size());
  EXPECT_TRUE(errorFile.completed());
  EXPECT_FALSE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture, ErrorFile_WarningsAndCrash) {
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/WarningsAndCrash.err");

  ErrorFile errorFile(path);
  ASSERT_EQ(static_cast<unsigned>(9), errorFile.warnings().size()) << "Number of warnings mistmatch for error file at '" << path << "'.";
  ;
  EXPECT_EQ("Output:PreprocessorMessage=\"EPXMLPreProc2\" has the following Warning condition:\n Reordered Verts for ULC convention",
            errorFile.warnings()[0]);
  EXPECT_EQ("In AirLoopHVAC RTU9_CAV there is unbalanced exhaust air flow.\n  During Warmup, Environment=FORT_WORTH TX USA TMY2-03927 WMO#=722596, "
            "at Simulation time=01/01 11:00 - 11:15\n   Unless there is balancing infiltration / ventilation air flow, this will result in\n   load "
            "due to induced outdoor air being neglected in the simulation.",
            errorFile.warnings()[8]);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.fatalErrors().size());
  EXPECT_FALSE(errorFile.completed());
  EXPECT_FALSE(errorFile.completedSuccessfully());
}

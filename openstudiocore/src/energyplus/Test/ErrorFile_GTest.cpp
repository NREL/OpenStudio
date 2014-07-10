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

#include "../../utilities/core/Logger.hpp"

#include <resources.hxx>

#include <sstream>

using openstudio::energyplus::ErrorFile;

TEST_F(EnergyPlusFixture,ErrorFile_NoErrorsNoWarnings)
{
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/NoErrorsNoWarnings.err");

  ErrorFile errorFile(path);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.warnings().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.fatalErrors().size());
  EXPECT_TRUE(errorFile.completed());
  EXPECT_TRUE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture,ErrorFile_Warnings)
{
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/Warnings.err");

  ErrorFile errorFile(path);
  ASSERT_EQ(static_cast<unsigned>(1), errorFile.warnings().size());
  EXPECT_EQ("Weather file location will be used rather than entered Location object.\n ..Location object=USA CO-BOULDER\n ..Weather File Location=Denver Centennial  Golden   Nr CO USA TMY3 WMO#=724666\n ..due to location differences, Latitude difference=[0.28] degrees, Longitude difference=[7.00E-002] degrees.\n ..Time Zone difference=[0.0] hour(s), Elevation difference=[11.93] percent, [195.00] meters.",
            errorFile.warnings()[0]);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.fatalErrors().size());
  EXPECT_TRUE(errorFile.completed());
  EXPECT_TRUE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture,ErrorFile_SevereErrors)
{
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/SevereErrors.err");

  ErrorFile errorFile(path);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.warnings().size());
  ASSERT_EQ(static_cast<unsigned>(26), errorFile.severeErrors().size());
  EXPECT_EQ("Out of range value Numeric Field#1 (Lighting Level), value=-1527598.22490, range={>=0}, in LIGHTS=ZN_1_FLR_1_SEC_1_LIGHTS", 
            errorFile.severeErrors()[0]);
  EXPECT_EQ("IP: Out of \"range\" values and/or blank required fields found in input", 
            errorFile.severeErrors()[25]);
  ASSERT_EQ(static_cast<unsigned>(1), errorFile.fatalErrors().size());
  EXPECT_EQ("IP: Errors occurred on processing IDF file. Preceding condition(s) cause termination.", 
            errorFile.fatalErrors()[0]);
  EXPECT_TRUE(errorFile.completed());
  EXPECT_FALSE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture,ErrorFile_WarningsAndSevere)
{
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/WarningsAndSevere.err");

  ErrorFile errorFile(path);
  ASSERT_EQ(static_cast<unsigned>(46), errorFile.warnings().size());
  EXPECT_EQ("Output:PreprocessorMessage=\"EPXMLPreProc2\" has the following Warning conditions:\n Requested glazing exceeds available area for\n B6CCD5_window_1.  Reducing sill height to fit.", 
            errorFile.warnings()[0]);
  EXPECT_EQ(static_cast<unsigned>(8), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(1), errorFile.fatalErrors().size());
  EXPECT_TRUE(errorFile.completed());
  EXPECT_FALSE(errorFile.completedSuccessfully());
}

TEST_F(EnergyPlusFixture,ErrorFile_WarningsAndCrash)
{
  openstudio::path path = resourcesPath() / openstudio::toPath("energyplus/ErrorFiles/WarningsAndCrash.err");

  ErrorFile errorFile(path);
  ASSERT_EQ(static_cast<unsigned>(9), errorFile.warnings().size());
  EXPECT_EQ("Output:PreprocessorMessage=\"EPXMLPreProc2\" has the following Warning condition:\n Reordered Verts for ULC convention", 
            errorFile.warnings()[0]);
  EXPECT_EQ("In AirLoopHVAC RTU9_CAV there is unbalanced exhaust air flow.\n  During Warmup, Environment=FORT_WORTH TX USA TMY2-03927 WMO#=722596, at Simulation time=01/01 11:00 - 11:15\n   Unless there is balancing infiltration / ventilation air flow, this will result in\n   load due to induced outdoor air being neglected in the simulation.", 
            errorFile.warnings()[8]);
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.severeErrors().size());
  EXPECT_EQ(static_cast<unsigned>(0), errorFile.fatalErrors().size());
  EXPECT_FALSE(errorFile.completed());
  EXPECT_FALSE(errorFile.completedSuccessfully());
}



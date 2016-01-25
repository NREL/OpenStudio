/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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
#include "IdfFixture.hpp"
#include "../Workspace.hpp"
#include "../WorkspaceObject.hpp"

#include "../ValidityReport.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <iostream>
#include <sstream>

using openstudio::ValidityReport;
using openstudio::Workspace;
using openstudio::WorkspaceObject;
using openstudio::IddFileType;
using openstudio::IddFile;
using openstudio::IdfObject;
using openstudio::IddObjectType;
using openstudio::StrictnessLevel;
using openstudio::DataError;

TEST_F(IdfFixture,ValidityReport_WithCustomIdd) 
{
  Workspace workspace;
  EXPECT_EQ(IddFileType::OpenStudio, workspace.iddFileType().value());
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
  workspace.addObject(IdfObject(IddObjectType::OS_Building));
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));

  std::stringstream ss;
  workspace.iddFile().print(ss);
  boost::optional<IddFile> iddFile = IddFile::load(ss);
  ASSERT_TRUE(iddFile);

  Workspace workspace2(*iddFile, StrictnessLevel::None);
  EXPECT_EQ(IddFileType::UserCustom, workspace2.iddFileType().value());
  workspace2.addObjects(workspace.toIdfFile().objects());
  EXPECT_EQ(workspace.objects().size(), workspace2.objects().size());
  EXPECT_TRUE(workspace2.isValid(StrictnessLevel::Draft));
  ValidityReport report = workspace2.validityReport(StrictnessLevel::Draft);
  LOG(Debug,"Validity report for workspace2: " << std::endl << report);
}


/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

TEST_F(IdfFixture, ValidityReport_WithCustomIdd) {
  Workspace workspace(StrictnessLevel::Draft, IddFileType::OpenStudio);
  EXPECT_EQ(IddFileType::OpenStudio, workspace.iddFileType().value());

  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));
  workspace.addObject(IdfObject(IddObjectType::OS_Building));
  EXPECT_TRUE(workspace.isValid(StrictnessLevel::Draft));

  std::stringstream ss;
  workspace.iddFile().print(ss);
  boost::optional<IddFile> iddFile = IddFile::load(ss);
  ASSERT_TRUE(iddFile);

  Workspace workspace2(*iddFile, StrictnessLevel::Minimal);
  EXPECT_EQ(IddFileType::UserCustom, workspace2.iddFileType().value());
  workspace2.addObjects(workspace.toIdfFile().objects());
  EXPECT_EQ(workspace.objects().size(), workspace2.objects().size());
  EXPECT_TRUE(workspace2.isValid(StrictnessLevel::Draft));
  ValidityReport report = workspace2.validityReport(StrictnessLevel::Draft);
  LOG(Debug, "Validity report for workspace2: " << '\n' << report);
}

/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  Workspace workspace(StrictnessLevel::Draft, IddFileType::OpenStudio);
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


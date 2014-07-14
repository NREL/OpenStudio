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
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../LocalProcessCreator.hpp"
#include "../RubyJobUtils.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/core/ApplicationPathHelpers.hpp"

#include "../../../utilities/filetypes/EpwFile.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OS_TimeDependentValuation_FieldEnums.hxx>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <boost/filesystem/path.hpp>

#include "../../../ruleset/OSArgument.hpp"

#include <resources.hxx>

using namespace openstudio;


TEST_F(RunManagerTestFixture, OSResultLoading)
{
  openstudio::runmanager::RunManager rm;
  openstudio::runmanager::Workflow wf;
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("OSResultLoadingTest");
  openstudio::path rubyscriptfile = resourcesPath() / openstudio::toPath("runmanager/create_os_result.rb");

  openstudio::runmanager::RubyJobBuilder rubyjobbuilder;
  rubyjobbuilder.setScriptFile(rubyscriptfile);
  rubyjobbuilder.setIncludeDir(getOpenStudioRubyIncludePath());
  rubyjobbuilder.addToWorkflow(wf);

  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), 
        rubyExePath().parent_path(), openstudio::path(),
        openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path());

  wf.add(tools);

  boost::filesystem::remove_all(outdir); // Clean up test dir before starting

  openstudio::runmanager::Job j = wf.create(outdir);
  rm.enqueue(j, true);

  rm.waitForFinished();

  openstudio::runmanager::JobErrors e = j.errors();

  EXPECT_EQ(openstudio::ruleset::OSResultValue(openstudio::ruleset::OSResultValue::Fail), e.result);

  ASSERT_EQ(2u, e.allErrors.size());

  EXPECT_EQ(openstudio::runmanager::ErrorType(openstudio::runmanager::ErrorType::Error), e.allErrors[0].first);
  EXPECT_EQ("Error1", e.allErrors[0].second);

  EXPECT_EQ(openstudio::runmanager::ErrorType(openstudio::runmanager::ErrorType::Warning), e.allErrors[1].first);
  EXPECT_EQ("Warning1", e.allErrors[1].second);

}


TEST_F(RunManagerTestFixture, OSResultLoading_Success)
{
  openstudio::runmanager::RunManager rm;
  openstudio::runmanager::Workflow wf;
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("OSResultLoadingTest");
  openstudio::path rubyscriptfile = resourcesPath() / openstudio::toPath("runmanager/create_os_result_success.rb");

  openstudio::runmanager::RubyJobBuilder rubyjobbuilder;
  rubyjobbuilder.setScriptFile(rubyscriptfile);
  rubyjobbuilder.setIncludeDir(getOpenStudioRubyIncludePath());
  rubyjobbuilder.addToWorkflow(wf);

  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), 
        rubyExePath().parent_path(), openstudio::path(),
        openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path(), openstudio::path());

  wf.add(tools);

  boost::filesystem::remove_all(outdir); // Clean up test dir before starting

  openstudio::runmanager::Job j = wf.create(outdir);
  rm.enqueue(j, true);

  rm.waitForFinished();

  openstudio::runmanager::JobErrors e = j.errors();

  EXPECT_EQ(openstudio::ruleset::OSResultValue(openstudio::ruleset::OSResultValue::Success), e.result);

  ASSERT_EQ(0u, e.allErrors.size());

}

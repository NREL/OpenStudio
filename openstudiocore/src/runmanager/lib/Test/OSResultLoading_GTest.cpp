/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "RunManagerTestFixture.hpp"
#include <runmanager/Test/ToolBin.hxx>
#include "../JobFactory.hpp"
#include "../RunManager.hpp"
#include "../LocalProcessCreator.hpp"
#include "../RubyJobUtils.hpp"

#include "../../../model/Model.hpp"
#include "../../../model/WeatherFile.hpp"

#include "../../../utilities/core/Application.hpp"
#include "../../../utilities/core/ApplicationPathHelpers.hpp"

#include "../../../utilities/filetypes/EpwFile.hpp"
#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/Workspace.hpp"
#include "../../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <boost/filesystem/path.hpp>

#include "../../../ruleset/OSArgument.hpp"

#include <resources.hxx>

using namespace openstudio;


TEST_F(RunManagerTestFixture, OSResultLoading)
{
  openstudio::Application::instance().application(false);
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
        rubyExePath().parent_path(), openstudio::path());

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
  openstudio::Application::instance().application(false);
  openstudio::runmanager::RunManager rm;
  openstudio::runmanager::Workflow wf;
  openstudio::path outdir = openstudio::tempDir() / openstudio::toPath("OSResultLoadingTestSuccess");
  openstudio::path rubyscriptfile = resourcesPath() / openstudio::toPath("runmanager/create_os_result_success.rb");

  openstudio::runmanager::RubyJobBuilder rubyjobbuilder;
  rubyjobbuilder.setScriptFile(rubyscriptfile);
  rubyjobbuilder.setIncludeDir(getOpenStudioRubyIncludePath());
  rubyjobbuilder.addToWorkflow(wf);

  openstudio::runmanager::Tools tools 
    = openstudio::runmanager::ConfigOptions::makeTools(energyPlusExePath().parent_path(), openstudio::path(), openstudio::path(), 
        rubyExePath().parent_path(), openstudio::path());

  wf.add(tools);

  boost::filesystem::remove_all(outdir); // Clean up test dir before starting

  openstudio::runmanager::Job j = wf.create(outdir);
  rm.enqueue(j, true);

  rm.waitForFinished();

  openstudio::runmanager::JobErrors e = j.errors();

  EXPECT_EQ(openstudio::ruleset::OSResultValue(openstudio::ruleset::OSResultValue::Success), e.result);

  ASSERT_EQ(0u, e.allErrors.size());

}
